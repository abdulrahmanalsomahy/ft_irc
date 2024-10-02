/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendMessages.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:27:59 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:27:59 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void	Server::sendMessage(Client &client, std::string message) {
	if (!message.empty() && message[message.size() - 1] != '\n')
		message += '\n';
	send(client.getClientPollfd().fd, message.c_str(), message.length(), 0);
}

void	Server::broadcastMessage(std::map<std::string, Client> map, Client& client, std::string channelName, std::string type, std::string textToBeSent) {
	std::map<std::string, Client>::iterator it = map.begin();
	while (it != map.end()) {
		if(it->second.getNick() != client.getNick() || type != "PRIVMSG"){
			sendMessage(it->second, GENMESSAGE(client, inet_ntoa(client.getClientAddress().sin_addr), channelName, type, textToBeSent));
		}
		it++;
	}
}

void	Server::broadcastMessage(std::map<std::string, Client> map, Client& client, std::string type, std::string textToBeSent) {
	for (std::map<std::string, Client>::iterator it = map.begin(); it != map.end(); it++) {
		if(it->second.getNick() != client.getNick() || type != "PRIVMSG")
			sendMessage(it->second, GENUSERMESSAGE(client, inet_ntoa(client.getClientAddress().sin_addr), type, textToBeSent));
	}
}

void	Server::broadcastMessage(std::map<std::string, Client> map, std::string message) {
	for (std::map<std::string, Client>::iterator it = map.begin(); it != map.end(); it++)
			sendMessage(it->second, message);
}

//Debug function
void	Server::printAllClients(void) {
	std::vector<Client>::iterator it = this->_clientVector.begin();
	while (it != this->_clientVector.end())
	{
		it->print();
		it++;
	}
}

void	Server::printAllChannels(void) {
	std::list<Channel>::iterator it = this->_channel_list.begin();
	while (it != this->_channel_list.end())
	{
		it->print();
		it++;
	}
}
