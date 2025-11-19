/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmartin <nmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 22:51:07 by nmartin           #+#    #+#             */
/*   Updated: 2025/11/18 16:58:53 by nmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int client(Data *data)
{
	struct addrinfo	*addrinfo;
	int				client;

	addrinfo = data->getAddrinfo();
	client = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);
	if (client == -1)
		data->exitError();
	if (connect(client, addrinfo->ai_addr, addrinfo->ai_addrlen) == -1)
		data->exitError();
	return (client);
}

int	main(int ac, char **av)
{
	Data	data;

	if (ac != 2)
	{
		std::cerr << "Error: Bad arguments!\nUsage: ./webserv [configuration file]" << std::endl;
		exit(2);
	}
	(void)av;
	//parsing conf file//JOUDY au charbon
	data.setAddrinfo();
	data.addListener();
	data.pollLoop();
	data.clean();
}