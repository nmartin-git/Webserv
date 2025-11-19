/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmartin <nmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 23:04:14 by nmartin           #+#    #+#             */
/*   Updated: 2025/11/18 19:25:28 by nmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

Connection::Connection() : _write_offset(0), _close (false)
{
	_fd.fd = -1;
    _fd.events = 0;
    _fd.revents = 0;
}

Connection::Connection(struct pollfd &fd) : _fd(fd), _write_offset(0), _close (false)
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
	ssize_t	received(0);

	_read_buf.clear();
	while (true)
	{
		received = recv(_fd.fd, buffer, BUFFER_SIZE - 1, MSG_DONTWAIT);
		if (received > 0)
		{
			buffer[received] = '\0';
			_read_buf += buffer;
		}
		else
			break;
	}
}

void	Connection::requestData(void)
{
	std::istringstream	flx(_read_buf);
	std::string			line;

	if (std::getline(flx, line))
	{
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		std::istringstream	request(line);
		request >> _method >> _uri >> _version;
		// std::cout << "Method: " << _method << " URI: " << _uri << " Version: " << _version << std::endl;
	}
	if (_read_buf.find("Connection: close") != std::string::npos)
		_close = true;
}

void	Connection::pollOut(void)
{
	std::string				response;

	recvData();
}

void	Connection::pollIn(void)
{
	recvData();
	std::cout << _read_buf << std::endl;
	requestData();
	if (_method == "GET")
		get();
}

bool	Connection::closeRequest(void)
{
	return (_close);
}