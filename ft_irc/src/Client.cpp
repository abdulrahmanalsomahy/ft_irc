/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 20:24:17 by abdabdul          #+#    #+#             */
/*   Updated: 2024/04/13 20:24:22 by abdabdul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"


Client::Client(void) {
	_status = CONNECTED;
	_nick = "*";
	_username = "*";
}

Client::Client(Client const& src) {
	*this = src;
}

Client::~Client(void) {
}

Client&	Client::operator=(Client const& rhs) {
	if (this != &rhs) {
		this->_clientSocket = rhs._clientSocket;
		this->_clientAddress = rhs._clientAddress;
		this->_clientPollfd = rhs._clientPollfd;
		this->_nick = rhs._nick;
		this->_username = rhs._username;
		this->_status = rhs._status;
		this->_buffer = rhs._buffer;
	}
	return *this;
}

/* === GETTERS - SETTERS === */
void Client::setClientSocket(int const& socket) {
	this->_clientSocket = socket;
}

void Client::setClientAddress(struct sockaddr_in const& address) {
	this->_clientAddress = address;
}

void Client::setClientPollfd_fd(int const& fd) {
	this->_clientPollfd.fd = fd;
}

void Client::setClientPollfd_events(short const& event) {
	this->_clientPollfd.events = event;
	this->_clientPollfd.revents = 0;
}

void	Client::setNick(std::string nick) {
	this->_nick = nick;
}

void	Client::setUsername(std::string username) {
	this->_username = username;
}

void	Client::setStatus(int status) {
	this->_status = status;
}

int&	Client::getClientSocket(void) {
	return this->_clientSocket;
}

struct sockaddr_in&	Client::getClientAddress(void) {
	return this->_clientAddress;
}

pollfd&	Client::getClientPollfd(void) {
	return this->_clientPollfd;
}

std::string&	Client::getNick(void) {
	return this->_nick;
}

std::string&	Client::getUsername(void) {
	return (this->_username);
}

std::string&	Client::getBuffer(void) {
	return this->_buffer;
}

void	Client::appendBuffer(std::string const& str) {
	this->_buffer += str;
}

int	Client::getStatus(void) {
	return (this->_status);
}

void	Client::print(void) {
	std::cout << LGREEN << "-----Printing Client:-----" << WHITE << std::endl;
	std::cout << LGREEN << "Name:\t|" << this->_username << WHITE << std::endl;
	std::cout << LGREEN << "Nick:\t|" << this->_nick << WHITE << std::endl;
	std::cout << LGREEN << "Status:\t|" << this->_status << WHITE << std::endl;
	std::cout << LGREEN << "Buffer:\t|" << this->_buffer << WHITE << std::endl;
}
