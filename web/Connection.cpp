/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmartin <nmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 23:04:14 by nmartin           #+#    #+#             */
/*   Updated: 2025/11/16 22:33:48 by nmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

Connection::Connection() : _write_offset(0)
{
	_fd.fd = -1;
    _fd.events = 0;
    _fd.revents = 0;
}

Connection::Connection(struct pollfd &fd) : _fd(fd), _write_offset(0)
{
	_read_buf.reserve(8162);
	_write_buf.reserve(8162);
}

Connection::~Connection()
{
}

void	Connection::sendData(std::string &data)
{
	size_t	total(0);
	size_t	n;

	while (total < data.size())
	{
        n = send(_fd.fd, data.c_str() + total, data.size() - total, 0);
		if (n > 0)
            total += n;
		// else
		// {
		// 	//TODO handle error
		// }
	}
}

void	Connection::recvData(void)
{
	char	buffer[BUFFER_SIZE];
	size_t	received(0);

	_read_buf.clear();
	while (true)
	{
		received = recv(_fd.fd, &buffer, BUFFER_SIZE - 1, MSG_DONTWAIT);
		if (received > 0)
		{
			buffer[received] = '\0';
			_read_buf += buffer;
		}
		else
			break;
		return ;
	}
}

void	Connection::sendResponse(void)
{
	std::string response;
	response = "HTTP/1.1 200 OK\r\n";
	response += "Date: Tue, 11 Nov 2025 17:19:30 GMT\r\n";
	response += "Server: MonServeur/1.0\r\n";
	response += "Content-Type: text/html; charset=utf-8\r\n";
	response += "Content-Length: 62\r\n";
	response += "Connection: keep-alive\r\n";
	response += "\r\n";
	response += "<html><head><title>Home</title></head><body>Welcome!</body></html>";
	sendData(response);
	_fd.events = POLLOUT;
	_write_offset += 0;
}

void	Connection::pollOut(void)
{
	std::string				response;

	recvData();
}

void	Connection::pollIn(void)
{
	std::string				request;

	recvData();
	std::cout << "\"" << _read_buf << "\"" <<std::endl;
	if (_read_buf.compare(0, 16, "GET / HTTP/1.1\r\n") == 0)
		sendResponse();
	std::cout << "pollin" <<std::endl;
}
