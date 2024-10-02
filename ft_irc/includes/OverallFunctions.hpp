/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OverallFunctions.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 19:46:01 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 19:46:01 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <cstring>
# include <map>
# include <list>
# include <sstream>
# include <ctime>
# include <sstream>
# include "Colors.hpp"

bool	isValidChannelName(std::string const &name);
std::list<std::string>	splitString(std::string const& stringToSplit, char const& delimiter);
std::string				currentTimeString(void);
