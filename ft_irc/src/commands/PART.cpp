/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:23:28 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:23:28 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void	Server::part(Client &client, t_ircMessage &params){
	if (client.getStatus() < REGISTERED) {
	sendMessage(client, ERR_NOTREGISTERED(client.getNick()));
	return;
	}
	if (params.parameters.size() < 2) {
		sendMessage(client, ERR_NEEDMOREPARAMS(client.getNick(), params.command));
		return ;
	}
	std::list<std::string>				to_kick_from = splitString(params.parametersVector[0], ',');
	std::list<std::string>::iterator	it_to_kick_from = to_kick_from.begin();
	while (it_to_kick_from != to_kick_from.end()) {
		//--->We found on official Server no Character that triggers this error
		if (!isValidChannelName(*it_to_kick_from)) {
			sendMessage(client, ERR_BADCHANMASK(client.getNick(), *it_to_kick_from));
			return ;
		}
		std::list<Channel>::iterator it_chan = this->_channel_list.begin();
		while (it_chan != this->_channel_list.end()) {
			//client is no Member of the Channel
			if (it_chan->getName() == *it_to_kick_from && !it_chan->isMember(client.getNick())) {
				sendMessage(client, ERR_NOTONCHANNEL(client.getNick(), it_chan->getName()));
				return ;
			}

			if (*it_to_kick_from != it_chan->getName())
				it_chan++;
			else
				break;
		}
		std::string	textToBeSent = "";
		//extract message
		if (params.parameters.size() >= 2) {
			size_t	spacePos = params.parameters.find(" ", params.parameters.find(" ") + 1);
			if (spacePos != std::string::npos)
				textToBeSent = client.getNick() + " :" + params.parameters.substr(spacePos + 1, params.parameters.size());
		}
		if (it_chan == this->_channel_list.end()) {
			sendMessage(client, ERR_NOSUCHCHANNEL(client.getNick(), *it_to_kick_from));
			return ;
		} else {
			std::vector<Client>::iterator it_client = getClient(client.getNick());
			broadcastMessage(it_chan->getAllMember(), client, it_chan->getName(), "PART", textToBeSent);
			if (it_chan->isOperator(*it_client))
				it_chan->removeOperator(*it_client);
			if (it_chan->isUser(*it_client))
				it_chan->removeUser(*it_client);
		}
		it_to_kick_from++;
	}
}