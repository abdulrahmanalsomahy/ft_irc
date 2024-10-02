/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:23:16 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:23:16 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void	Server::kick(Client &client, t_ircMessage& params) {
	if (client.getStatus() < REGISTERED) {
		sendMessage(client, ERR_NOTREGISTERED(client.getNick()));
		return;
	}
	if (params.parameters.size() < 2) {
		sendMessage(client, ERR_NEEDMOREPARAMS(client.getNick(), params.command));
		return ;
	}
	std::list<std::string>				to_kick_from = splitString(params.parametersVector[0], ',');
	std::list<std::string>				to_kick_users = splitString(params.parametersVector[1], ',');
	std::list<std::string>::iterator	it_to_kick_users = to_kick_users.begin();
	while (it_to_kick_users != to_kick_users.end()) {
		std::list<std::string>::iterator	it_to_kick_from = to_kick_from.begin();
		while (it_to_kick_from != to_kick_from.end()) {
			//the name of channel is valid
			if (!isValidChannelName(*it_to_kick_from)) {
				sendMessage(client, ERR_BADCHANMASK(client.getNick(), *it_to_kick_from));
				return ;
			}
			std::list<Channel>::iterator it_chan = this->_channel_list.begin();
			while (it_chan != this->_channel_list.end()) {
				//client is no Member of the Channel
				if (it_chan->getName() == *it_to_kick_from && !it_chan->isMember(client.getNick())) {
					sendMessage(client, ERR_NOTONCHANNEL(*it_to_kick_users, it_chan->getName()));
					return ;
				}
				//Kicking Member is no Operator
				if (it_chan->getName() == *it_to_kick_from && !it_chan->isOperator(client)) {
					sendMessage(client, ERR_CHANOPRIVSNEEDED(client.getNick(), it_chan->getName()));
					return ;
				}
				//To be kicked User is no Member of the Channel
				if (it_chan->getName() == *it_to_kick_from && !it_chan->isMember(*it_to_kick_users)) {
					sendMessage(client, ERR_USERNOTINCHANNEL(client.getNick(), *it_to_kick_users, it_chan->getName()));
					return ;
				}
				if (*it_to_kick_from != it_chan->getName())
					it_chan++;
				else
					break;
			}
			std::string	textToBeSent = *it_to_kick_users + " :" + *it_to_kick_users;
			//extract message
			if (params.parameters.size() > 2) {
				size_t	spacePos = params.parameters.find(" ", params.parameters.find(" ") + 1);
				if (spacePos != std::string::npos)
					textToBeSent = *it_to_kick_users + " :" + params.parameters.substr(spacePos + 1, params.parameters.size());
			}
			if (it_chan == this->_channel_list.end()) {
				sendMessage(client, ERR_NOSUCHCHANNEL(client.getNick(), *it_to_kick_from));
				return ;
			} else {
				std::vector<Client>::iterator it_client = getClient(*it_to_kick_users);
				broadcastMessage(it_chan->getAllMember(), client, it_chan->getName(), "KICK", textToBeSent);
				if (it_chan->isOperator(*it_client))
					it_chan->removeOperator(*it_client);
				if (it_chan->isUser(*it_client))
					it_chan->removeUser(*it_client);
			}
			it_to_kick_from++;
		}
		it_to_kick_users++;
	}
}
