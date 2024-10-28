# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/17 16:07:39 by agrimald          #+#    #+#              #
#    Updated: 2024/10/28 17:10:06 by agrimald         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CPP = c++
CPPFLAGS = -Wall -Werror -Wextra -std=c++98 -fsanitize=address -I./inc -I./inc/commands -I./src -I./src/commands
RM = rm -rf
CYAN = \033[96m
GREEN = \33[92m
YELLOW = \033[93m
MAGENTA = \033[35m
RED = \033[91m
END_COLOR = \033[0m

SRC = main.cpp src/Message.cpp src/Client.cpp \
	  src/Server.cpp src/Channel.cpp src/Utils.cpp \
	  src/commands/Pass.cpp src/commands/Quit.cpp \
	  src/commands/Nick.cpp src/commands/Join.cpp \
	  src/commands/Kick.cpp src/commands/ClasString.cpp \
	  src/commands/Privmsg.cpp src/commands/Topic.cpp \
	  src/commands/Invite.cpp src/commands/Part.cpp \
	  src/commands/Mode.cpp src/commands/Who.cpp
		

INC = inc/Channel.hpp inc/Client.hpp inc/Message.hpp \
	  inc/Server.hpp inc/Utils.hpp inc/commands/ClasString.hpp \
	  inc/commands/Invite.hpp inc/commands/Kick.hpp \
	  inc/commands/Nick.hpp inc/commands/Pass.hpp \
	  inc/commands/Quit.hpp inc/commands/Who.hpp \
	  inc/commands/Command.hpp inc/commands/Join.hpp \
	  inc/commands/Mode.hpp inc/commands/Part.hpp \
	  inc/commands/Privmsg.hpp inc/commands/Topic.hpp

OBJECTS = $(SRC:.cpp=.o)

all: banner $(NAME)

banner:
	@echo  "$(MAGENTA)\n"                                                                         
	@echo  "                 ▄▄                                                              "
	@echo  "               ▄▄█▀▀▀███▀▀▀▄▄▄                  :::::::::::                      "
	@echo  "           ▄▄▀▀▄▄▄▄▄▄▄▄▄██▄▄  ████▀▀                :+:                          "
	@echo  "         ▄▀    ▀█▄    ▀██▀   ▀██▄▄▀▄▄               +:+                          "
	@echo  "       ▄▀         ▀▀▀▀▀▀      ▀▄ ▀████              +#+                          "
	@echo  "      ▄▀      ▄█ ▄▄▄▄▀▀▀▀▀▀▀▄▄▄ ▀▀▀▀▀ █▄            +#+                          "
	@echo  "     ▄▀        █▄       ▀▀▀▀▄▄▄▀█▄     █            #+#                          "
	@echo  "     █                         ▀ ▀█     █       ###########                      "
	@echo  "     █                            ▀█    █                                        "
	@echo  "     ▀▄                                ▄█                  :::::::::             "
	@echo  "      █▄                               █                   :+:    :+:            "
	@echo  "       ▀▄                             █                    +:+    +:+            "
	@echo  "        ▀█▄                         ▄▀                     +#++:++#:             "
	@echo  "           ▀▄▄                   ▄▄▀                       +#+    +#+            "
	@echo  "              ▀▀▄▄▄         ▄▄▄▀▀                          #+#    #+#            "
	@echo  "                   ▀▀▀▀▀█▀▀▀                               ###    ###            "
	@echo  "                       █████                                                     "
	@echo  "                      █ █   ██                                        ::::::::   "
	@echo  "                     █  ██    █                                      :+:    :+:  "
	@echo  "                    █    ██    █                                     +:+         "
	@echo  "                   █      ██    ██                                   +#+         "
	@echo  "                ▄██        ██    █▄                                  +#+         "
	@echo  "                ▄█          ██    █▄                                 #+#    #+#  "
	@echo  "                            ██                                        ########   "
	@echo  "                                                                                 "
	@echo "\n$(END_COLOR)"                                           

banner_fclean:
	@echo "$(GREEN)\n"
	@echo "                      ██████                         "
	@echo "                   ███      ███                      "
	@echo "                  █            █                     "
	@echo "                ██              ██                   "
	@echo "                █                ██                  "
	@echo "               █                  █                  "
	@echo "              █                    █                 "
	@echo "              ██████   ██████       █                "
	@echo "             █ ███      ████        █                "
	@echo "             █ ███      ████        █                "
	@echo "               ███      ████         █               "
	@echo "            █  ███      ███        ███               "
	@echo "            █                       ██               "
	@echo "            █            ████        █               "
	@echo "            █          ███████       █               "
	@echo "             █       ██████████       █              "
	@echo "             █      ████████████     █               "
	@echo "             █     ██████████  █     █               "
	@echo "              █       ███████ █  █ ██                "
	@echo "              █                   ██                 "
	@echo "               █                  █                  "
	@echo "                █                █                   "
	@echo "                 █              ██                   "
	@echo "       ██         ██           ████    ████████      "
	@echo "       █████        ██      ███   ██████████████     "
	@echo "         ███████        ██       █████               "
	@echo "            █████████        █████████████           "
	@echo "                 █████████████████   ██  ██████      "
	@echo "                                 █    █       ██     "
	@echo "                                 █    ██     █       "
	@echo "                                 █     █     █ █     "
	@echo "                                 █     ██    █ █     "
	@echo "                            ██████ █   ██    █ █     "
	@echo "                         ████    █████ ██    ███     "
	@echo "                 █████████      ████████████████     "
	@echo "        █████████████████████████████████    █ █     "
	@echo "      ████████          █  ██████  ███████   █ █     "
	@echo "                         █        █     ████ █ █     "
	@echo "                          █      █        ████ █     "
	@echo "                           ██████           ████     "
	@echo "\n$(END_COLOR)"


$(NAME): $(OBJECTS)
		 @printf "$(CYAN)\n\nThe IRC is compiled!!!\n$(END_COLOR)\n"
		 @$(CPP) $(CPPFLAGS) -o $@ $^

%.o: %.cpp $(INC)
		@printf "$(YELLOW)Generating IRC objects... %-33.33s \r$(END_COLOR)" $@
		@$(CPP) $(CPPFLAGS) -c $< -o $@

fclean:  banner_fclean clean
		 $(RM) $(NAME) $(OBJECTS)

clean:   
		 @printf "$(RED)\nIRC DELETE :c!!!\n$(END_COLOR)\n"
		 @$(RM) $(OBJECTS)

re: clean fclean all

.PHONY: re clean fclean all
