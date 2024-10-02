/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:23:54 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:23:54 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void	Server::user(Client& client, t_ircMessage& params) {
	//check if a password was supplied
	if (client.getStatus() < AUTHENTICATED)
		return sendMessage(client, ERR_NOTREGISTERED(client.getNick()));
	// set username
	else if (client.getStatus() >= REGISTERED)
		return sendMessage(client, ERR_ALREADYREGISTERED(client.getNick()));
	else if (params.parametersList.size() == 0)
		client.setUsername("unknown");
	else
		client.setUsername(params.parametersVector[0]);

	// check registration status
	switch (client.getStatus())
	{
	case AUTHENTICATED:
		client.setStatus(USERGIVEN);
		break;
	case NICKGIVEN:
		client.setStatus(REGISTERED);
		sendMessage(client, RPL_WELCOME(client.getNick()));
		break;
	default:
		break;
	}
}
