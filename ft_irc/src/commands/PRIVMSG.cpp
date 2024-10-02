/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:23:47 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:23:47 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void	Server::privmsg(Client &client, t_ircMessage &params) {
	if (client.getStatus() < REGISTERED)
		return sendMessage(client, ERR_NOTREGISTERED(client.getNick()));

	std::string	textToBeSent;
	size_t		spacePos = params.parameters.find(" ");

	// no text to be sent
	if (spacePos != std::string::npos)
		textToBeSent = params.parameters.substr(spacePos + 1, params.parameters.size());
	if (spacePos == std::string::npos || textToBeSent.empty()) {return(sendMessage(client, ERR_NOTEXTTOSEND(client.getNick())));}
	std::list<std::string>	targetList = splitString(params.parameters.substr(0, spacePos), ',');
	for (std::list<std::string>::iterator itTarget = targetList.begin(); itTarget != targetList.end(); ++itTarget) {
		// no valid channel name
		if (isValidChannelName(*itTarget)) {
			std::list<Channel>::iterator itChannel = this->_channel_list.begin();
			while (itChannel != this->_channel_list.end()) {
				if (itChannel->getName() == *itTarget) {
					if (!itChannel->isMember(client.getNick())) { return(sendMessage(client, ERR_CANNOTSENDTOCHAN(client.getNick(), itChannel->getName())));}
					broadcastMessage(itChannel->getAllMember(), client, itChannel->getName(), "PRIVMSG", textToBeSent);
					break;
				}
				itChannel++;
			}
			// no existing channel name
			if (itChannel == this->_channel_list.end()) {return (sendMessage(client, ERR_NOSUCHCHANNEL(client.getNick(), *itTarget)));}
		}
		else {
			// send message to user
			std::vector<Client>::iterator itClient = this->_clientVector.begin();
			while(itClient != this->_clientVector.end()) {
				if(itClient->getNick() == *itTarget) {
					std::map<std::string, Client> target;
					target[itClient->getNick()] =  *itClient;
					broadcastMessage(target, client, "PRIVMSG", textToBeSent);
					break;
				}
				itClient++;
			}
			// nickname not found
			if (itClient == this->_clientVector.end()) {return(sendMessage(client, ERR_NOSUCHNICK(client.getNick(), *itTarget)));}
		}
	}
}