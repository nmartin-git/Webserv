/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dataTransfer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmartin <nmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 19:47:12 by nmartin           #+#    #+#             */
/*   Updated: 2025/11/11 20:23:35 by nmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	sendData(int fd, std::string &data)
{
	char	buffer[READ_BUFFER];
	size_t	total(0);
	size_t	n;

	while (total < data.size())
	{
        n = send(fd, data.c_str() + total, data.size() - total, 0);
		if (n > 0)
            total += n;
		// else
		// {
		// 	//TODO handle error
		// }
	}
}

std::string	&recvData(int fd)
{
	char		buffer[1024];
	size_t		total(0);
	size_t		n;
	std::string	data(0);

	while (recv(fd, &buffer, 1024, 0))
	{
		data += buffer;
	}
	return (data);
}