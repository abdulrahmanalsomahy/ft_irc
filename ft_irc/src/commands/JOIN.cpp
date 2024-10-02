/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:23:14 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:23:14 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"


void	Server::join(Client &client, t_ircMessage& params) {
	if (client.getStatus() < REGISTERED) {
		sendMessage(client, ERR_NOTREGISTERED(client.getNick()));
		return;
	}
	if (params.parameters.empty()) {
		sendMessage(client, ERR_NEEDMOREPARAMS(client.getNick(), params.command));
		return ;
	}
	if (params.parametersList.size() == 1)
		params.parametersList.push_back("");
	std::list<std::string> tojoin = splitString(params.parametersList.front(), ',');
	std::list<std::string> tojoinpw = splitString(params.parametersList.back(), ',');
	std::list<std::string>::iterator it_join = tojoin.begin();
	std::list<std::string>::iterator it_joinpw = tojoinpw.begin();
	while (it_join != tojoin.end()) {
		//the name of channel is valid
		if (!isValidChannelName(*it_join)) {
			sendMessage(client, ERR_BADCHANMASK(client.getNick(), *it_join));
			return ;
		}
		std::list<Channel>::iterator it_chan = this->_channel_list.begin();
		while (it_chan != this->_channel_list.end()) {
			std::map<std::string, Client> invites = it_chan->getInviteList();
			std::map<std::string, Client>::iterator clientinvited = invites.find(client.getNick());
			if (it_chan->getName() == *it_join && it_chan->isMember(client.getNick()))
				return ;
			if (it_chan->getName() == *it_join && it_chan->isInviteOnly() == true && clientinvited == invites.end()) {
				sendMessage(client, ERR_INVITEONLYCHAN(client.getNick(), *it_join));
				return ;
			}
			//Channellimit reached
			if (it_chan->getName() == *it_join && it_chan->getLimit() > 0 && it_chan->getAmountOfAll() >= it_chan->getLimit()) {
				sendMessage(client, ERR_CHANNELISFULL(client.getNick(), *it_join));
				return ;
			}
			//possible problem if no password exist in the command, also valid iterator of password is not secured
			if (it_chan->getName() == *it_join && ((it_joinpw == tojoinpw.end() && it_chan->getPassword() != "") || (it_joinpw != tojoinpw.end() && it_chan->getPassword() != *it_joinpw))) {
				sendMessage(client, ERR_BADCHANNELKEY(client.getNick(), *it_join));
				return ;
			}
			if (it_chan->getName() == *it_join && it_chan->isBanned(client)) {
				sendMessage(client, ERR_BANNEDFROMCHAN(client.getNick(), *it_join));
				return ;
			}
			if (*it_join != it_chan->getName())
				it_chan++;
			else
				break;
		}
		//case we send Nosuchchannel, because we create
		// if (it_chan == this->_channel_list.end()) {
		// 	sendMessage(client, ERR_NOSUCHCHANNEL(*it_join));
		// 	return ;
		// }
		if (it_chan == this->_channel_list.end())
		{
			Channel newCH(*it_join);
			newCH.setOperator(client);
			if (it_joinpw != tojoinpw.end())
				newCH.setPassword(*it_joinpw);
			this->_channel_list.push_back(newCH);
			broadcastMessage(newCH.getOperators(), client, newCH.getName(), "JOIN", "");
			sendMessage(client, USERLIST(inet_ntoa(this->_serverAddress.sin_addr), client, *it_join, newCH.genUserlist()));
			sendMessage(client, USERLISTEND(inet_ntoa(this->_serverAddress.sin_addr), client, *it_join));
		} else {
			it_chan->setUser(client);
			// if ( it_chan->isInviteOnly())
				it_chan->removeInvite(client);
			broadcastMessage(it_chan->getAllMember(), client, it_chan->getName(), "JOIN", "");
			if (it_chan->getTopic().empty())
				sendMessage(client, RPL_NOTOPIC(client.getNick(), it_chan->getName()));
			else
				sendMessage(client, RPL_TOPIC(client.getNick(), it_chan->getName(), it_chan->getTopic()));
			sendMessage(client, USERLIST(inet_ntoa(this->_serverAddress.sin_addr), client, it_chan->getName(), it_chan->genUserlist()));
			sendMessage(client, USERLISTEND(inet_ntoa(this->_serverAddress.sin_addr), client, it_chan->getName()));
		}
		it_join++;
		it_joinpw++;
	}

}