/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:23:31 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:23:31 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void	Server::pass(Client &client, t_ircMessage &params) {
	if (client.getStatus() >= AUTHENTICATED)
		sendMessage(client, ERR_ALREADYREGISTERED(client.getNick()));
	else if (params.parameters.empty())
		sendMessage(client, ERR_NEEDMOREPARAMS(client.getNick(), params.command));
	else if (this->_serverPassword == params.parameters)
	{
		client.setStatus(AUTHENTICATED);
	}
	else
		sendMessage(client, ERR_PASSWDMISMATCH(client.getNick()));
}