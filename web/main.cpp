/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmartin <nmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 22:51:07 by nmartin           #+#    #+#             */
/*   Updated: 2025/12/04 22:27:47 by nmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

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

int client(Data *data)
{
	struct addrinfo	*addrinfo;
	int				client;

	addrinfo = data->getAddrinfo();
	client = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);
	if (client == -1)
		data->exitError();
	if (connect(client, addrinfo->ai_addr, addrinfo->ai_addrlen) == -1)
		data->exitError();
	return (client);
}

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
