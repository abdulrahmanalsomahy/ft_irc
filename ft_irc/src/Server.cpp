/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:27:31 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:27:31 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server(void) {
}

Server::Server(int port, std::string password) : _serverPort(port), _serverPassword(password) {
	this->_run = true;
	this->_commandMap["JOIN"] = &Server::join;
	this->_commandMap["CAP"] = &Server::cap;
	this->_commandMap["PASS"] = &Server::pass;
	this->_commandMap["NICK"] = &Server::nick;
	this->_commandMap["USER"] = &Server::user;
	this->_commandMap["QUIT"] = &Server::quit;
	this->_commandMap["PRIVMSG"] = &Server::privmsg;
	this->_commandMap["MODE"] = &Server::mode;
	this->_commandMap["KICK"] = &Server::kick;
	this->_commandMap["TOPIC"] = &Server::topic;
	this->_commandMap["INVITE"] = &Server::invite;
	this->_commandMap["PART"] = &Server::part;
}

Server::Server(Server const &src) {
	*this = src;
}

Server::~Server(void) {
	this->_fds.clear();
	this->_clientVector.clear();
	this->_channel_list.clear();
	this->_commandMap.clear();
}

Server &Server::operator=(Server const &rhs) {
	if (this != &rhs)
	{
		this->_serverPort = rhs._serverPort;
		this->_serverPassword = rhs._serverPassword;
		this->_serverSocket = rhs._serverSocket;
		this->_optval = rhs._optval;
		this->_serverAddress = rhs._serverAddress;
		this->_fds = rhs._fds;
		this->_serverPollfd = rhs._serverPollfd;
		this->_clientVector = rhs._clientVector;
		this->_channel_list = rhs._channel_list;
		this->_commandMap = rhs._commandMap;
	}
	return *this;
}

/* === EXCEPTIONS === */
char const *Server::SocketException::what() const throw() { return ("Error creating socket."); }

char const *Server::BindException::what() const throw() { return ("Error binding socket."); }

char const *Server::ListenException::what() const throw() { return ("Error listening on socket."); }

char const *Server::AcceptException::what() const throw() { return ("Error accepting connection."); }

char const *Server::PollException::what() const throw() { return ("Error in poll."); }

char const *Server::FdException::what() const throw() { return ("Error with the connection."); }

/* === FUNCTIONS === */
void	Server::serverSetup(void)
{
	try
	{
		this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (this->_serverSocket < 0)
		{
			throw Server::SocketException();
		}
		this->_optval = 1;
		if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &this->_optval, sizeof(this->_optval)) < 0)
		{
			throw Server::SocketException();
		}
		if ((fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK)) < 0)
		{
			throw Server::SocketException();
		}
		this->_serverAddress.sin_family = AF_INET;
		this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
		this->_serverAddress.sin_port = htons(this->_serverPort);
		if (bind(this->_serverSocket, (struct sockaddr *)&this->_serverAddress, sizeof(this->_serverAddress)) < 0)
		{
			throw Server::BindException();
		}
		if (listen(this->_serverSocket, 5) < 0)
		{
			throw Server::ListenException();
		}
		this->_serverPollfd.fd = this->_serverSocket;
		this->_serverPollfd.events = POLLIN;
		this->_fds.push_back(this->_serverPollfd);

		std::cout << CYAN << "Server startup completed. Listening on port:" << this->_serverPort << WHITE << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

/*Starts up the server and waits in poll until an event is registered.*/
void	Server::serverStart(void)
{
	char buffer[1024];
	int ret;
	int rb;

	try
	{
		serverSetup();
		while (this->_run)
		{
			ret = poll(this->_fds.data(), this->_fds.size(), -1);
			if (ret == -1)
				throw PollException();
			// check server
			if (this->_fds[0].revents & POLLIN)
				addClient();

			// check clients
			for (size_t i = 1; i < this->_fds.size(); ++i)
			{
				std::vector<t_ircMessage> commands; // contains all valid client messages
				std::vector<Client>::iterator clientIterator = this->_clientVector.begin() + i - 1;

				if ((this->_fds[i].revents & POLLIN)) {
					bzero(buffer, sizeof(buffer));
					rb = recv(this->_fds[i].fd, buffer, sizeof(buffer), 0);

					if (ret > 0 && rb > 0 && handleClient(buffer, clientIterator, commands))
					{
						for (std::vector<t_ircMessage>::iterator it = commands.begin(); it != commands.end(); ++it)
						{
							std::map<std::string, void (Server::*)(Client &, t_ircMessage &)>::iterator function = this->_commandMap.find(it->command);
							if (function != _commandMap.end())
								(this->*(function->second))(*clientIterator, *it);
						}
						commands.clear();
					}
					if (!clientIterator->getStatus() || this->_fds[i].revents & (POLLERR | POLLHUP) || ret <= 0 || rb <= 0)
					{
						std::cout << PURPLE << "Client " << ntohs(clientIterator->getClientAddress().sin_port) << " disconnected" << WHITE << std::endl;
						removeClient(*clientIterator);
						_clientVector.erase(clientIterator);
						close(this->_fds[i].fd);
						this->_fds.erase(this->_fds.begin() + i);
						--i;
					}
				}
			}
		}
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void	Server::removeClient(Client& client) {
	for (std::list<Channel>::iterator it = _channel_list.begin(); it != _channel_list.end(); it++)
		if (it->isMember(client.getNick())) {
			// remove client from channel
			it->removeInvite(client);
			it->removeUser(client);
			it->removeOperator(client);
			// broadcast to all relevant channels
			broadcastMessage(it->getAllMember(), ":" + client.getNick() + "!~" + client.getUsername() + "@irc42" + " QUIT :Client Quit");
		}
}

std::vector<Client>::iterator	Server::getClient(std::string& nick) {
	std::vector<Client>::iterator	it = this->_clientVector.begin();
	while (it != this->_clientVector.end() && it->getNick() != nick)
		it++;
	return it;
}

std::list<Channel>::iterator	Server::getChannel(std::string& channelName) {
	std::list<Channel>::iterator	it = this->_channel_list.begin();
	while (it != this->_channel_list.end() && it->getName() != channelName)
 		it++;
	return it;
}
