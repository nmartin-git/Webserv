/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmartin <nmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 20:54:02 by nmartin           #+#    #+#             */
/*   Updated: 2025/10/30 17:46:27 by nmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>

#define MAX_FDS 1

class	Data
{
	public:

		Data();
		~Data();
		void	setAddrinfo(void);
		void	addSocket(void);
		int		getFd(int index);
		void	clean(void);
		void	exit(int status);
		void	exitError(void);

	private:

		struct addrinfo	*_addrinfo;
		int				_fds[MAX_FDS];
		int				_fdsNbr;
};

//---socket.cpp---

void	socketHandling(Data *data);