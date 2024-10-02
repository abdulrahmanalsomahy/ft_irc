/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:23:11 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:23:12 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void	Server::invite(Client& client, t_ircMessage& params) {
	if (client.getStatus() < REGISTERED) {
		sendMessage(client, ERR_NOTREGISTERED(client.getNick()));
		return;
	}
	// too few parameters
	if (params.parametersList.size() < 2)
		return (sendMessage(client, ERR_NEEDMOREPARAMS(client.getNick(), params.command)));

	// search invited user
	std::list<Channel>::iterator		itChannel = this->_channel_list.begin();
	std::vector<Client>::iterator		itClient = _clientVector.begin();
	std::string							channelName = params.parametersVector[1];
	std::string							clientName = params.parametersVector[0];
	for (; itClient != _clientVector.end(); itClient++) {
		if (itClient->getNick() == clientName) {
			break;
		}
	}
	// invited nickname does not exist
	if (itClient == this->_clientVector.end())
		return(sendMessage(client, ERR_NOSUCHNICK(client.getNick(), clientName)));

	// channel name is not valid
	if(!isValidChannelName(channelName))
		return (sendMessage(client, ERR_NOSUCHCHANNEL(client.getNick(), channelName)));
	//search channel
	for (; itChannel != _channel_list.end(); itChannel++)
		if (itChannel->getName() == channelName)
			break;
	// channel does not exist
	if (itChannel == _channel_list.end())
		return (sendMessage(client, ERR_NOSUCHCHANNEL(client.getNick(), channelName)));

	// channel is invite only and issuer is not operator
	if (itChannel->isInviteOnly() && !itChannel->isOperator(client))
		return (sendMessage(client, ERR_CHANOPRIVSNEEDED(clientName, channelName)));
	// channel is not invite only and issuer is not a channel opertator and is not a channel member
	else if(!itChannel->isInviteOnly() && !itChannel->isMember(client.getNick()))
		return (sendMessage(client, ERR_NOTONCHANNEL(clientName, channelName)));
	// invited nickname is already on channel
	if (itChannel->isMember(clientName))
		return (sendMessage(client, ERR_USERONCHANNEL(clientName, channelName)));

	itChannel->setInviteList(*itClient);

	std::string	textToBeSent = ":" + channelName;
	sendMessage(*itClient, GENMESSAGE(client, inet_ntoa(client.getClientAddress().sin_addr), itClient->getNick(), "INVITE", textToBeSent));
	sendMessage(client, RPL_INVITING(client.getNick(), itClient->getNick(), itChannel->getName()));
}
