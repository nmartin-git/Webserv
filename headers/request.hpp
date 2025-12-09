/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmartin <nmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 15:04:18 by nmartin           #+#    #+#             */
/*   Updated: 2025/12/09 18:11:28 by nmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

#define BUFFER_SIZE 1024

typedef	struct s_upload
{
	std::string	filename;
	size_t		contentLength;
}				t_upload;

class	Connection
{
	public:

	Connection();
	Connection(struct pollfd *fd);
	~Connection();
	void		sendData(void);
	void		recvData(void);
	void		requestData(void);

	void		send404(void);
	void		sendResponse(std::string filename);
	void		sendIcon(void);
	void		get(void);

	void		getFilename(t_upload *data, size_t headersLength);
	void		upload(void);
	void		post(void);

	void		pollOut(void);
	void		pollIn(void);
	bool		closeRequest(void);
	
	private:
	
	struct pollfd	*_fd;
	std::string 	_read_buf;
	std::string		_write_buf;
	size_t			_write_offset;
	size_t			_expected_length;
	std::string 	_method;
	std::string 	_uri;
	std::string 	_version;
	bool			_close;
};
