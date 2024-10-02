/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:23:22 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:23:22 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void	Server::nick(Client &client, t_ircMessage &params) {
	// check if a password was supplied
	if (client.getStatus() < AUTHENTICATED)
		return sendMessage(client, ERR_NOTREGISTERED(client.getNick()));
	else if (client.getStatus() >= REGISTERED)
		return sendMessage(client, ERR_ALREADYREGISTERED(client.getNick()));

	// check if there is a name
	if (params.parametersList.size() == 0)
		sendMessage(client, ERR_NONICKNAMEGIVEN(client.getNick()));
	// MISSING: check character validity ERR_ERRONEUSNICKNAME

	//check uniqueness
	for (std::vector<Client>::iterator it = this->_clientVector.begin(); it != this->_clientVector.end(); it++) {
		if (it->getStatus() >= AUTHENTICATED && it->getNick() == params.parametersVector[0]) {
			sendMessage(client, ERR_NICKNAMEINUSE(params.parametersVector[0]));
			return;
		}
	}

	// check registration status
	switch (client.getStatus())
	{
		case AUTHENTICATED:
			client.setStatus(NICKGIVEN);
			break;
		case USERGIVEN:
			client.setStatus(REGISTERED);
			sendMessage(client, RPL_WELCOME(params.parametersVector[0]));
			break;
		default:
			break;
	}
	// set nickname
	client.setNick(params.parametersVector[0]);
}