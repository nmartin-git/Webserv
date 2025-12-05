/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmartin <nmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 13:52:30 by nmartin           #+#    #+#             */
/*   Updated: 2025/12/04 23:55:17 by nmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

void	Connection::getFilename(t_upload *data)
{
	std::istringstream	flx(_read_buf);
	std::string			line;
	std::string			next_line;
	std::string			filename;
	size_t				index;
	size_t				end;

	index = _read_buf.find("Content-Length: ");
	if (index == std::string::npos)
		return ;
	end = _read_buf.find("\r\n", index);
	if (end == std::string::npos)
		return ;
	data->contentLength = atoi(_read_buf.substr(index + 16, end - index).c_str());
	while (std::getline(flx, line) && line.find("boundary=") == std::string::npos);
	index = line.find("boundary=");
	if (index == std::string::npos)
		return ;
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);
	next_line = line.substr(index + strlen("boundary="));
	std::cout << "==//=" << next_line << "==//=" << std::endl;
	while (std::getline(flx, line) && line.find(next_line) == std::string::npos);
	std::cout << "==//=" << line << "==//=" << std::endl;
	std::getline(flx, line);
	index = line.find("filename=\"");
	if (index == std::string::npos)
		return ;
	index += 10;
	end = line.find("\"", index);
    if (end == std::string::npos)
		return ;
	filename = line.substr(index, end - index);
	filename = "data/" + filename;
	filename.insert(filename.find("."), getTimestamp());
	data->filename = filename;
}

void	Connection::upload(void)
{
	size_t		start;
	size_t		end;
	size_t		file_size;
	t_upload	data;

	getFilename(&data);
	std::cout << "Filename: " << data.filename << "   Content-Length: " << data.contentLength << std::endl;
	if (data.filename.empty())
		return ;
	
	// Trouver le début du contenu du fichier (après les headers multipart)
	start = _read_buf.find("\r\n\r\n");
	if (start == std::string::npos)
		return ;
	start = _read_buf.find("\r\n\r\n", start + 4);  // 2ème occurrence
	if (start == std::string::npos)
		return ;
	start += 4;  // Sauter le \r\n\r\n
	
	// Trouver la fin du fichier (avant le boundary de fin)
	end = _read_buf.find("\r\n--", start);
	if (end == std::string::npos)
		return ;
	
	file_size = end - start;
	std::cout << "File start: " << start << " end: " << end << " size: " << file_size << std::endl;
	
	std::ofstream file(data.filename.c_str(), std::ios::binary);
	if (!file)
	{
    	std::cerr << "Error: file creation failed" << std::endl;
   		return;
	}
	
	file.write(_read_buf.c_str() + start, file_size);
	file.close();
	std::cout << "File uploaded successfully: " << data.filename << std::endl;
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