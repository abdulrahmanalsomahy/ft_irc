/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:23:51 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:23:51 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void	Server::topic(Client& client, t_ircMessage& params) {
	if (client.getStatus() < REGISTERED)
		return sendMessage(client, ERR_NOTREGISTERED(client.getNick()));

	// need more parameters
	if (params.parametersList.empty()) {return (sendMessage(client, ERR_NEEDMOREPARAMS(client.getNick(), params.command)));}
	std::list<std::string>::iterator itParams = params.parametersList.begin();
	// no such channel
	std::list<Channel>::iterator itChannel = this->getChannel(*itParams);
	if (!isValidChannelName(*itParams) || itChannel == this->_channel_list.end())
		return(sendMessage(client, ERR_NOSUCHCHANNEL(client.getNick(), *itParams)));
	//not on channel
	if (!itChannel->isMember(client.getNick()))
		return (sendMessage(client, ERR_NOTONCHANNEL(client.getNick(), *itParams)));
	// no operator with protected channel
	if (itChannel->isTopicRestricted() && !itChannel->isOperator(client) && params.parametersList.size() > 1)
		return (sendMessage(client, ERR_CHANOPRIVSNEEDED(client.getNick(), itChannel->getName())));
	// no topic argument passed
	if (params.parametersList.size() == 1) {
		// no previously set topic to return
		if(itChannel->getTopic().empty()) {return(sendMessage(client, RPL_NOTOPIC(client.getNick(), itChannel->getName())));}
		// return previously set topic
		else {
			sendMessage(client, RPL_TOPIC(client.getNick(), itChannel->getName(), itChannel->getTopic()));
			sendMessage(client, RPL_TOPICWHOTIME(client.getNick(), itChannel->getName(), itChannel->getTopicSetby(), itChannel->getTopicSetat()));
		}
	}
	// clear topic
	else if (params.parametersList.size() == 2 && *(++itParams) == ":") {
		itChannel->clearTopic();
		std::map<std::string, Client> members;
		std::map<std::string, Client>::iterator it = members.begin();
		for (; it != members.end(); ++it) {
			sendMessage(it->second, RPL_TOPIC(it->first, itChannel->getName(), itChannel->getTopic()));
		}
	}
	// set new topic
	else if (params.parametersList.size() >=2) {
		size_t pos = 0;
		std::string	topic;
		pos = params.parameters.find(' ', pos);
		if (pos != std::string::npos)
			topic = params.parameters.substr(pos + 1, params.parameters.size());
		itChannel->setTopic(topic);
		itChannel->setTopicSetat();
		itChannel->setTopicSetby(client.getNick());
		std::map<std::string, Client> members = itChannel->getAllMember();
		std::map<std::string, Client>::iterator it = members.begin();
		for (; it != members.end(); ++it) {
			sendMessage(it->second, RPL_TOPIC(it->first, itChannel->getName(), itChannel->getTopic()));
			sendMessage(it->second, RPL_TOPICWHOTIME(it->first, itChannel->getName(), itChannel->getTopicSetby(), itChannel->getTopicSetat()));
		}
	}
}