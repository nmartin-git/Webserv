/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmartin <nmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 21:31:37 by nmartin           #+#    #+#             */
/*   Updated: 2025/12/10 19:14:10 by nmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

void	Connection::send404(void)
{
	_response.setStatus(404);
	_response.addHeader("Content-Type", "text/html");
	_response.setBody("<h1>404 - File Not Found</h1>");
	_write_buf.clear();
	_write_buf = _response.build();
    sendData();
	_fd->events = POLLOUT;
}

void	Connection::sendResponse(std::string filename)
{
	std::cout << "reponse" << std::endl;
	std::ifstream	file(filename.c_str(), std::ios::binary);
	std::string		length;
	std::ostringstream size;

	if (!file)
		return (send404());
	std::string content((std::istreambuf_iterator<char>(file)),
                    		std::istreambuf_iterator<char>());
	file.close();
	size << content.size();
	length = size.str();
	_response.setStatus(200);
	_response.addHeader("Content-Type", _response.get_content_type(_uri));
	_response.setBody(content);
	// _write_buf += "Content-Length" + length + "\r\n";//TODO faire content length
	_write_buf.clear();
	_write_buf = _response.build();
	sendData();
}

void	Connection::sendIcon(void)
{
	std::cout << "icon" << std::endl;
	std::ifstream	icon("website/Imran.ico", std::ios::binary);
	std::string		length;
	std::ostringstream size;

	if (!icon)
		return (send404());
	std::string content((std::istreambuf_iterator<char>(icon)),
                    		std::istreambuf_iterator<char>());
	icon.close();
	size << content.size();
	length = size.str();
	_write_buf.clear();
	_write_buf = "HTTP/1.1 200 OK\r\n";
    _write_buf += "Content-Type: image/x-icon\r\n";
    _write_buf += "Content-Length: " + length + "\r\n";
    _write_buf += "Connection: keep-alive\r\n";
    _write_buf += "\r\n";
    _write_buf += content;
    sendData();
}

void	Connection::get(void)
{
	// std::cout << _uri <<std::endl;
	/*if (is_cgi(_uri))
        start_cgi(client, _uri);
	else*/ if (_uri == "/")
		sendResponse("website/home.html");
	else if (_uri == "/favicon.ico")
		sendIcon();
	else if (_uri == "/upload")
		sendResponse("website/upload.html");
	// else if (_uri == "/cgi")
	// 	execCgi();
}
