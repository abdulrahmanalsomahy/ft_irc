# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abdabdul <abdabdul@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/13 20:28:27 by abdabdul          #+#    #+#              #
#    Updated: 2024/04/13 20:28:27 by abdabdul         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC_COMMANDS = src/commands/CAP.cpp src/commands/PASS.cpp src/commands/NICK.cpp src/commands/USER.cpp \
			src/commands/QUIT.cpp src/commands/JOIN.cpp src/commands/PRIVMSG.cpp src/commands/INVITE.cpp \
			src/commands/TOPIC.cpp src/commands/KICK.cpp src/commands/MODE.cpp src/commands/PART.cpp

SRC = src/main.cpp src/Server.cpp src/Client.cpp src/Channel.cpp  src/SendMessages.cpp \
		src/OverallFunctions.cpp src/Parsing.cpp $(SRC_COMMANDS)

OBJS = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all