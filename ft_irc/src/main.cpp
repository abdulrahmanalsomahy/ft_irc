/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:24:28 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:24:28 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void    handle_port(char *_port)
{
    std::string check;
    int i_port;

    check = _port;
    if (check.find_first_not_of("-+0123456789") != std::string::npos)
        throw std::out_of_range("Error input: _port must contain only digits");
    i_port = std::atoi(_port);
    if (i_port >= 0 && i_port <= 1023)
		throw std::out_of_range("_ports 0 - 1023 are _reserved _ports! Please use a different _port.");
	if (i_port < 0 || i_port > 65535) 
		throw std::out_of_range("Error input: _port is out of range [1024; 65535]");
}

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 3)
            throw std::runtime_error("retry: ./ircserv <port> <password>");

        handle_port(argv[1]);
		int port = atoi(argv[1]);
		std::string	password = static_cast<std::string>(argv[2]);
		Server	ircServer(port, password);
		ircServer.serverStart();
    }
    catch(const std::exception &e){
		std::cerr << e.what()<< std::endl;
	}
	return (0);
}