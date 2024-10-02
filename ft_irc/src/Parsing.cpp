/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:28:12 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:28:12 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void	Server::addClient(void) {
	Client newClient;
	socklen_t clientAddressLen = sizeof(newClient.getClientAddress());

	newClient.setClientSocket(accept(this->_serverSocket, (struct sockaddr *)&newClient.getClientAddress(), &clientAddressLen));
	if (newClient.getClientSocket() < 0)
	{
		throw AcceptException();
	}
	std::cout << ORANGE << "New client connected : " << inet_ntoa(newClient.getClientAddress().sin_addr) << ":" << ntohs(newClient.getClientAddress().sin_port) << WHITE << std::endl;
	newClient.setClientPollfd_fd(newClient.getClientSocket());
	newClient.setClientPollfd_events(POLLIN);
	newClient.setStatus(CONNECTED);
	this->_clientVector.push_back(newClient);
	this->_fds.push_back(newClient.getClientPollfd());
}

bool	Server::inputParse(std::string& message, t_ircMessage &clientCommand) {
	size_t pos = 0;
	if (!message.empty() && message[message.size() - 1] == '\r')
		message = message.substr(0, message.size() - 1);
	if (!message.empty() && message[pos] == ':')
	{
		pos++;
		size_t prefixEnd = message.find(' ', pos);
		if (prefixEnd == std::string::npos)
			return false;
		clientCommand.prefix = message.substr(pos, prefixEnd - pos);
		pos = prefixEnd + 1;
	}

	size_t commandEnd = message.find(' ', pos);
	if (commandEnd == std::string::npos) {
		clientCommand.command = message.substr(pos, commandEnd);
		return true;
	}
	clientCommand.command = message.substr(pos, commandEnd - pos);
	pos = commandEnd + 1;
	clientCommand.parameters = message.substr(pos, message.size() - 2);

	/* remove initial ':' if present, e.g. KVirc PASS :pw */
	if (!clientCommand.parameters.empty() && clientCommand.parameters.size() > 1 && clientCommand.parameters[0] == ':')
		clientCommand.parameters = clientCommand.parameters.substr(1, clientCommand.parameters.size());
	/* split parameters string on space, into list of strings */
	if (!clientCommand.parameters.empty())
		clientCommand.parametersList = splitString(clientCommand.parameters, ' ');
	return true;
}

bool	Server::handleClient(char *buffer, std::vector<Client>::iterator &clientIterator, std::vector<t_ircMessage> &commands) {
	std::istringstream	iss(buffer);
	std::string 		tmpBuffer;
	t_ircMessage 		clientCommand;
	bool				executable = true;

	while (getline(iss, tmpBuffer)) {
		if (iss.eof()) {
				std::cout << "DEBUG: found EoF\n";
			clientIterator->appendBuffer(tmpBuffer);
			executable = false;
		}
		else {
			clientIterator->appendBuffer(tmpBuffer);
			if (inputParse(clientIterator->getBuffer(), clientCommand)) {
				std::vector<std::string>	copy(clientCommand.parametersList.begin(), clientCommand.parametersList.end());
				clientCommand.parametersVector = copy;
				commands.push_back(clientCommand);
			}
			tmpBuffer.clear();
			clientIterator->getBuffer().clear();
			executable = true;
		}
	}
	return executable;
}
