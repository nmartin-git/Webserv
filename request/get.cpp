/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmartin <nmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 21:31:37 by nmartin           #+#    #+#             */
/*   Updated: 2025/11/18 19:24:57 by nmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

void	Connection::send404(void)
{
	std::string response;

    response = "HTTP/1.1 404 Not Found\r\n";
    response += "Content-Length: 0\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
	_close = true;
    sendData(response);
}

void	Connection::sendResponse(void)
{
	std::cout << "reponse" << std::endl;
	std::string response;
	response = "HTTP/1.1 200 OK\r\n";
	response += "Date: Tue, 11 Nov 2025 17:19:30 GMT\r\n";
	response += "Server: MonServeur/1.0\r\n";
	response += "Content-Type: text/html; charset=utf-8\r\n";
	response += "Content-Length: 62\r\n";
	response += "Connection: close\r\n";
	response += "\r\n";
	response += "<html><head><title>Home</title></head><body>Welcome!</body></html>";
	sendData(response);
	_fd.events = POLLOUT;
	_write_offset += 0;
	_close = true;
}

void	Connection::sendIcon(void)
{
	std::cout << "icon" << std::endl;
	std::ifstream	icon("Imran.ico", std::ios::binary);
	std::string 	response;
	std::string		lenght;

	if (!icon)
		return (send404());
	std::string content((std::istreambuf_iterator<char>(icon)),
                    		std::istreambuf_iterator<char>());
	icon.close();
	std::ostringstream size;
	size << content.size();
	lenght = size.str();
	response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: image/x-icon\r\n";
    response += "Content-Length: " + lenght + "\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += content;
    sendData(response);
}

void	Connection::get(void)
{
	// std::cout << _uri <<std::endl;
	if (_uri == "/")
		sendResponse();
	else if (_uri == "/favicon.ico")
		sendIcon();
}
