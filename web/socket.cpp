/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmartin <nmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 23:03:38 by nmartin           #+#    #+#             */
/*   Updated: 2025/10/30 17:44:52 by nmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	socketHandling(Data *data)
{
	struct sockaddr_storage their_addr;
	socklen_t				addr_size;
	int						fd;

	addr_size = sizeof(their_addr);
	fd = accept(data->getFd(0), (struct sockaddr *)&their_addr, &addr_size);
	close(fd);
}
