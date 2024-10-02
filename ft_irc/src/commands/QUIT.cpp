/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:23:49 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:23:49 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void	Server::quit(Client &client, t_ircMessage &params) {
	(void)params;
	sendMessage(client, ERROR(params.parameters));
	client.setStatus(DISCONNECTED);
}