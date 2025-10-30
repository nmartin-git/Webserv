/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmartin <nmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:02:50 by nmartin           #+#    #+#             */
/*   Updated: 2025/10/30 18:31:08 by nmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

#define PORT "8080"

Data::Data() : _addrinfo(NULL), _fdsNbr(0)
{
}

Data::~Data()
{
}

void	Data::setAddrinfo(void)
{
	int				status;
	struct addrinfo	hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	status = getaddrinfo(NULL, PORT, &hints, &this->_addrinfo);
	if (status != 0)
	{
		std::cerr << "Error: getaddrinfo: " << gai_strerror(status) << std::endl;
		std::exit(status);
	}
}

void	Data::addSocket()
{
	// if (_fdsNbr >= MAX_FDS)//handle
	_fds[_fdsNbr] = socket(_addrinfo->ai_family, _addrinfo->ai_socktype, _addrinfo->ai_protocol);
	if (_fds[_fdsNbr] == -1)
	{
		std::cerr << "A" << std::endl;
		exitError();
	}
	int yes = 1;
	setsockopt(_fds[_fdsNbr], SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	if (bind(_fds[_fdsNbr], _addrinfo->ai_addr, _addrinfo->ai_addrlen) == -1)
	{
		std::cerr << "B" << std::endl;
		exitError();
	}
	if (connect(_fds[_fdsNbr], _addrinfo->ai_addr, _addrinfo->ai_addrlen) == -1)
	{
		std::cerr << "C" << std::endl;
		exitError();
	}
	if (listen(_fds[_fdsNbr], 1) == -1)
	{
		std::cerr << "D" << std::endl;
		exitError();
	}
	_fdsNbr++;
}

int	Data::getFd(int index)
{
	return (_fds[index]);
}

void	Data::clean(void)
{
	if(_addrinfo)
		freeaddrinfo(_addrinfo);
	for (int i = 0; i < _fdsNbr; i++)
		close(_fds[i]);
}

void	Data::exit(int	status)
{
	clean();
	std::exit(status);
}

void	Data::exitError(void)
{
	clean();
	std::cerr << "Error: " << strerror(errno) << std::endl;
	std::exit(errno);
}