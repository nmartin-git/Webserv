// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   main.cpp                                           :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: efranco <efranco@student.42.fr>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/10/30 14:07:19 by efranco           #+#    #+#             */
// /*   Updated: 2025/12/02 18:22:28 by efranco          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <map>
#include <sys/wait.h>
#include <fstream>
#include "Client.hpp"
#include "Env.hpp"
#include "request.hpp"
#include "webserv.hpp"

#define FLAG 0

std::string	getTimestamp(void)
{
    struct tm time_info;
    struct timeval tv;
    char buf[32];
	std::time_t t = std::time(NULL);

    localtime_r(&t, &time_info);

    int year2 = (time_info.tm_year + 1900) % 100;
    int month = time_info.tm_mon + 1;
    int day = time_info.tm_mday;
    int seconds = time_info.tm_sec;
    int milli = 0;
    if (gettimeofday(&tv, NULL) == 0) {
        milli = static_cast<int>((tv.tv_usec / 1000) % 1000);
    } else {
        milli = 0;
    }
    std::sprintf(buf, "_upload_%02d%02d%02d%02d%03d", year2, month, day, seconds, milli);
    return std::string(buf);
}

bool	is_extension_cgi(const std::string &str)
{
	return (str == ".php" || str == ".py" || str == ".sh");
}

bool	is_cgi(const std::string &str)
{
	size_t	i;

	i = str.find_last_of('.');
	if (i != std::string::npos)
	{
		std::string tmp = str.substr(i);
		if (is_extension_cgi(tmp))
			return (true);
	}
	return (false);
}

// void set_cgi_env(Client& client, const std::string &script_path)
// {
// 	setenv("REQUEST_METHOD", client.env.get_method().c_str(), 1);
//     setenv("SCRIPT_NAME", script_path.c_str(), 1);
//     setenv("QUERY_STRING", client.env. get_query_string().c_str(), 1);
//     setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
//     setenv("SERVER_SOFTWARE", "WebServ/1.0", 1);
//     setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
//     setenv("SERVER_NAME", "localhost", 1);
//     setenv("SERVER_PORT", "8080", 1);
//     setenv("REMOTE_ADDR", "127.0.0.1", 1);

//     if (! client.env.get_Cookie_string().empty())
//     {
//         setenv("HTTP_COOKIE", client.env.get_Cookie_string().c_str(), 1);
//     }

// 	std::map<std::string, std::string>& http_headers = client.env.get_headers();
// 	for(std::map<std::string, std::string>::iterator it = http_headers.begin(); it != http_headers.end(); ++it)
// 	{
// 		std::string env_name = client.env.transform_header_name(it->first);
// 		setenv(env_name.c_str(), it->second.c_str(), 1);
// 	}
// }

void	start_cgi(Client &client, const std::string &path)
{

    size_t derniere_barre = path.find_last_of('/');
    std::string nom_fichier = path.substr(derniere_barre + 1);
    std::string chemin_reel = "./" + nom_fichier;
	pid_t	pid;
	int		pipefd[2];
	char	*args[] = {(char *)"/usr/bin/python3", (char *)chemin_reel.c_str(), NULL};

	if (pipe(pipefd) == -1)
	{
		std::cerr << "error : pipe()" << std::endl;
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		std::cerr << "error : fork()" << std::endl;
		return ;
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			std::cerr << "error : dup2()" << std::endl;
			exit(1);
		}
		close(pipefd[1]);

		set_cgi_env(client, path);
		if (execve("/usr/bin/python3", args, environ) == -1)
		{
			std::cerr << "error : execve()" << std::endl;
			exit(1);
		}
	}
	if (pid > 0)
	{
		close(pipefd[1]);
		set_nonblocking(pipefd[0]);

		client.cgi = new CGIExecution();
		client.cgi->pid = pid;
		client.cgi->pipe_fd = pipefd[0];
		client.cgi->output = "";
		client.cgi->completed = false;
		client.state = EXECUTING_CGI;
	}
}

// void	handle_executing_cgi(Client &client, int fd)
// {
// 	char		buffer[4096];
// 	ssize_t		n;
// 	int			status;
// 	int			result;
// 	Response	res;

// 	if (client.cgi != NULL)
// 	{
// 		n = read(client.cgi->pipe_fd, buffer, sizeof(buffer));
// 		if (n > 0)
// 		{
// 			client.cgi->output.append(buffer, n);
// 		}
// 		if (n == 0)
// 		{
// 			std::cout << "EOF sur le pipe" << std::endl;
// 		}
// 		if (n == -1)
// 		{
// 			if (errno != EAGAIN && errno != EWOULDBLOCK)
// 			{
// 				std::cerr << "error : read()" << std::endl;
// 				return ;
// 			}
// 		}
// 		result = waitpid(client.cgi->pid, &status, WNOHANG);
// 		if (result > 0)
// 		{
// 			client.cgi->completed = true;
// 		}
// 		if (result == -1)
// 		{
// 			std::cerr << "error : waitpid()" << std::endl;
// 			return ;
// 		}
// 		if (client.cgi->completed == true)
// 		{

//     		parse_cgi_output(client.cgi);

//     		std::map<std::string, std::vector<std::string> > parsed_headers;

//     		if (client.cgi->has_headers)
//     		{
//         		parse_cgi_headers(client.cgi->cgi_headers, parsed_headers);
//     		}
//     		else
//     		{
//         		parsed_headers["Content-Type"].push_back("text/html");
//     		}

// 			std::string httpResponse = build_cgi_response(client.cgi, parsed_headers);
// 			send(fd, httpResponse.c_str(), httpResponse.length(), 0);

// 			close(client.cgi->pipe_fd);
// 			delete client.cgi;
// 			client.cgi = NULL;
// 			client.state = DONE;
// 		}
// 	}
// 	else
// 	{
// 		std::cerr << "Erreur : cgi est NULL" << std::endl;
// 		client.state = DONE;
// 		return ;
// 	}
// }

int	main(int ac, char **av)
{
	Data	data;

	if (ac != 2)
	{
		std::cerr << "Error: Bad arguments!\nUsage: ./webserv [configuration file]" << std::endl;
		exit(2);
	}
	(void)av;
	//parsing conf file//JOUDY au charbon
	data.setAddrinfo();
	data.addListener();
	data.pollLoop();
	data.clean();
}
