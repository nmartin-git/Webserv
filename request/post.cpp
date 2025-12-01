/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmartin <nmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 13:52:30 by nmartin           #+#    #+#             */
/*   Updated: 2025/12/01 15:47:42 by nmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

void	Connection::upload(void)
{
	
}

void	Connection::post(void)
{
	if (_read_buf.find("Transfer-Encoding: chunked") != _read_buf.npos)
	{
	//TODO send 411 Length Required
		return ;
	}
	if (_uri == "/upload")
		upload();
}