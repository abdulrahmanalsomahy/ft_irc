/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OverallFunctions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:28:05 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:28:05 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/OverallFunctions.hpp"

/*Channel name can't contain more than 200chars, spaces, commas, ^D or miss a & or # at the beginning*/
bool	isValidChannelName(std::string const &name)
{
	if (name.empty())
		return false;
	if (name.length() > 200)
		return false;
	if (name.find(' ', 0) != name.npos)
		return false;
	if (name.find(',', 0) != name.npos)
		return false;
	if (name.find('\a', 0) != name.npos)
		return false;
	if (name.length() > 0 && !(name[0] == '&' || name[0] == '#'))
		return false;
	return true;
}

/* splits the stringToBeSPlit on delimiter and returns a list of splits  */
std::list<std::string>	splitString(std::string const&stringToBeSplit, char const& delimiter) {
	std::list<std::string>	listOfSplits;
	std::istringstream		iss(stringToBeSplit);
	std::string				token;

	while(getline(iss, token, delimiter)) {listOfSplits.push_back(token);}
	return listOfSplits;
}

/* returns current timestamp as std::string */
std::string				currentTimeString(void) {
	time_t	currentTime = time(NULL);

	std::ostringstream oss;
	oss << currentTime;
	std::string	timeString = oss.str();
	return timeString;
}
