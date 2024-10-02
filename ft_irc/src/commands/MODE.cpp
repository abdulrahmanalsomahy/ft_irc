/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:23:19 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:23:19 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void	Server::mode(Client& client, t_ircMessage& params) {
	if (client.getStatus() < REGISTERED)
		return sendMessage(client, ERR_NOTREGISTERED(client.getNick()));

	//check if a channel mode is given
	if (params.parametersVector.size() < 1)
		return sendMessage(client, ERR_NEEDMOREPARAMS(client.getNick(), params.command));
	else if (params.parametersVector.size() == 1)
	//MISSING: add MODE #channel command here if you like
		return;

	std::string	channelname = params.parametersVector[0];
	//search channel
	if (isValidChannelName(channelname) == false)
		return sendMessage(client, ERR_BADCHANMASK(client.getNick(), channelname));

	std::list<Channel>::iterator channel = this->_channel_list.begin();
	while (channel->getName() != channelname)
		if (++channel == this->_channel_list.end())
			return sendMessage(client, ERR_NOSUCHCHANNEL(client.getNick(), channelname));

	//check if client is channel operator
	if (channel->isOperator(client) == false)
		return sendMessage(client, ERR_CHANOPRIVSNEEDED(client.getNick(), channelname));

	//check number of parameters
	if (enoughModeParameters(params) == false)
		return sendMessage(client, ERR_NEEDMOREPARAMS(client.getNick(), params.command));

	std::string	mode = params.parametersVector[1];
	bool		add = true;
	size_t		parameter = 2;
	//MISSING: check if there is no sign

	for (std::string::iterator s = mode.begin(); s != mode.end(); s++) {
		switch (*s) {
			case '+':
				add = true;
				break;
			case '-':
				add = false;
				break;
			case 'i':
				channel->setInviteOnly(add);
				break;
			case 't':
				channel->setRestrictTopic(add);
				break;
			case 'k':
				modeK(client, *channel, add, params.parametersVector[parameter++]);
				break;
			case 'o':
				modeO(client, *channel, add, params.parametersVector[parameter++]);
				break;
			case 'l':
				if (add == true)
					channel->setLimit(params.parametersVector[parameter++]);
				else
					channel->removeLimit();
				break;
			default:
				sendMessage(client, ERR_UNKNOWNMODE(client.getNick() + " " + mode, channel->getName()));
				return;
		}
	}
}

void	Server::modeO(Client& client, Channel& channel, bool add, std::string& target) {
	std::vector<Client>::iterator	it = getClient(target);
	if (it == this->_clientVector.end()) {
			sendMessage(client, ERR_NOSUCHNICK(client.getNick(), target));
			return;
	}

	if (add == true)
		channel.setOperatorStatus(*it);
	else
		channel.removeOperatorStatus(*it);
}

void	Server::modeK(Client& client, Channel& channel, bool add, std::string& password) {
	if (add == true) {
		if (channel.getPassword() != "")
			sendMessage(client, ERR_KEYSET(client.getNick(), channel.getName()));
		else
			channel.setPassword(password);
	} else {
		if (channel.getPassword() != password)
			sendMessage(client, ERR_PASSWDMISMATCH(client.getNick()));
		else
			channel.setPassword("");
	}
}

bool	Server::enoughModeParameters(t_ircMessage& params) {
	size_t		parameter = 2;
	std::string	mode = params.parametersVector[1];
	bool		add = true;

	for (std::string::iterator	c = mode.begin(); c != mode.end(); c++) {
		switch (*c)
		{
			case '+':
				add = true;
				break;
			case '-':
				add = false;
				break;
			case 'l':
				if (add == false)
					continue;
				//fallthrough
			case 'k':
			case 'o':
				if (parameter >= params.parametersVector.size())
					return (false);
				parameter++;
				break;
			default:
				break;
		}
	}
	return (true);
}
