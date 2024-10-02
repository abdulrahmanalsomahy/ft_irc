/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CAP.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:23:04 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:23:04 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void	Server::cap(Client &client, t_ircMessage &params) {
	if (params.parameters == "LS")
	{
		client.setStatus(CAP);
		sendMessage(client, RPL_CAP);
	}
	if (params.parameters == "END")
		client.setStatus(CONNECTED);
}