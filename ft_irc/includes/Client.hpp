/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 19:45:45 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 19:45:46 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <netinet/in.h>
# include <poll.h>
# include "Colors.hpp"

# define DISCONNECTED	0
# define CONNECTED		1
# define CAP			2
# define AUTHENTICATED	3
# define NICKGIVEN		4
# define USERGIVEN		5
# define REGISTERED		6

class Client {
	private:
		int					_clientSocket;
		struct sockaddr_in	_clientAddress;
		pollfd				_clientPollfd;
		std::string			_nick;
		std::string			_username;
		int					_status;
		std::string			_buffer;

	public:
		Client(void);
		Client(Client const& src);
		~Client(void);
		Client&	operator=(Client const& rhs);

		void	setClientSocket(int const& socket);
		void	setClientAddress(struct sockaddr_in const& address);
		void	setClientPollfd_fd(int const&);
		void	setClientPollfd_events(short const&);
		void	setNick(std::string nick);
		void	setUsername(std::string username);
		void	setStatus(int status);
		void	appendBuffer(std::string const& str);
		void	print(void);

		int&				getClientSocket(void);
		struct sockaddr_in&	getClientAddress(void);
		pollfd&				getClientPollfd(void);

		std::string&		getNick(void);
		std::string&		getUsername(void);
		int					getStatus(void);
		std::string&		getBuffer(void);
};
