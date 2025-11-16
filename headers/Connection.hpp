/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmartin <nmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 15:04:18 by nmartin           #+#    #+#             */
/*   Updated: 2025/11/16 22:25:52 by nmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

#define BUFFER_SIZE 1024

class	Connection
{
	public:

	Connection();
	Connection(struct pollfd &fd);
	~Connection();
	void		sendData(std::string &data);
	void		recvData(void);
	void		sendResponse(void);
	void		pollOut(void);
	void		pollIn(void);
	
	private:
	
	struct pollfd	_fd;
	std::string 	_read_buf;
	std::string		_write_buf;
	size_t			_write_offset;
	std::string 	_method;
	std::string 	_uri;
	std::string 	_http;
};
