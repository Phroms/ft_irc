/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 12:49:41 by agrimald          #+#    #+#             */
/*   Updated: 2024/10/23 15:48:00 by ojimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/commands/Mode.hpp"
# include "../../inc/Channel.hpp"

# define RPL_CHANNELMODES(servername, nickname, channel, flag) \
			(std::string(":" + servername + " 324 " + nickname + " " + channel + " " + modes)) // esto esta ok

		/* Controla que los usuarios sean OP(operadores - admin)
			MODE tendra los siguientes modos(modificaciones):

			. i: Set/remove Invite-only channel.
			. t: Set/remove the restrictions of the TOPIC command
				 to channel operators.
			. k: Set/remove the cannel key (password).
			. o: Give/take channel operator privilege.
			. l: Set/remove the user limit to channel.

			MODE: tiene acceso a la informacion de los canales y sus permisos
			Ejemplo de un comando en MODE:
			
				MODE #cannal +i -t +pk -pk
		*/

void Mode::execute(Server &server, Client &c, std::vector<std::string> args)
{
	Channel *channel = NULL;

	//  Validar si la solicitud del canal es correcta
	if (!validModeRequest(server, c, args, channel))
	{
		return;
	}

	// Si solo se pasa el nombre del canal, mostrar los modos actuales
	if (args.size() == 1)
	{
		std::string channelName = channel->getName();
		std::string modes = channel->getAllModes();

		std::string response = "MODE " + channelName + " " + modes + "\r\n";

		server.sendResponse(c.getFd(), RPL_CHANNELMODES(server.getServerName(), c.getNickname(), channelName, modes));
		std::cout << "Viene del mode excute: " << RPL_CHANNELMODES(server.getServerName(), c.getNickname(), channelName, modes) << std::endl;
		return ;
	}

	// Aplicar los cambios del modo
	applyModeChange(server, c, channel, args);
}

bool Mode::validModeRequest(Server &server, Client &c, std::vector<std::string> args, Channel *&channel)
{
	if (args.size() < 1)
	{
		std::string cmd = "MODE";
		server.message.sendMessage(c, server.message.getMessage(461, c, cmd));
		return false;
	}

	std::string channelName = args[0];
	channel = server.getChannel(channelName);

	if (args.size() < 1 || args[0].empty())
	{
		std::string cmd = "MODE";
		server.message.sendMessage(c, server.message.getMessage(461, c, cmd));
		return false;
	}
	if (!channel)
	{
		server.message.sendMessage(c, server.message.getMessage(482, c, channelName));
		return false;
	}

	// Verificar que el cliente sea un operador
	//std::cout << "Fisrt client: " << channel->isOperator(c) << std::endl;
	if (!channel->isOperator(c))
	{
		server.message.sendMessage(c, server.message.getMessage(482, c, channelName));
		return false;
	}
	return true;
}

void Mode::applyModeChange(Server &server, Client &c, Channel *channel, std::vector<std::string> args)
{
	bool addMode = true;
	for (size_t i = 1; i < args.size(); i++) // o ++i
	{
		std::string modeString = args[i];

		for (size_t j = 0; j < modeString.size(); j++) // o ++j
		{
			switch(modeString[j])
			{
				case '+':
					addMode = true;
					break;
				case '-':
					addMode = false;
					break;
				case 'i':
					handleInvitedOnly(channel, addMode, server, c);
					break;
				case 't':
					handleTopicMode(channel, addMode, server, c);
					break;
				case 'k':
					handleKeyMode(channel, addMode, server, c, args, i);
					break;
				case 'o':
					handleOperatorMode(channel, addMode, server, c, args, i);
					break;
				case 'l':
					handleLimitMode(channel, addMode, server, c, args, i);
					break;
				default:
					std::string cmd = "" + modeString[j];
					server.message.sendMessage(c, server.message.getMessage(472, c, cmd));
					break;
			}
	
		}
	}
}

// Modo de invitacion ("+i" o "-i")
void Mode::handleInvitedOnly(Channel *channel, bool addMode, Server &server, Client &c)
{
	channel->setInvitedOnly(addMode);
	broadcastModeChange(server, c, channel->getName(), (addMode ? "+i" : "-i"));
}

// Modo de restriccion de topico ("+t" o "-t")
void Mode::handleTopicMode(Channel *channel, bool addMode, Server &server, Client &c)
{
	channel->setTopicRestriction(addMode);
	broadcastModeChange(server, c, channel->getName(), (addMode? "+t" : "-t"));
}

// Modo de clave ("+k" o "-k")
void Mode::handleKeyMode(Channel *channel, bool addMode, Server &server, Client &c, std::vector<std::string> &args, size_t &index)
{
	if (addMode)
	{
		if (index + 1 < args.size())
		{
			std::string password = args[++index]; // o index++
			if (channel->getKey() == password)
			{
				std::string cmd = channel->getName();
				server.message.sendMessage(c, server.message.getMessage(467, c, cmd));
			}
			else
			{
				channel->setKey(password);
				broadcastModeChange(server, c, channel->getName(), "+k" + password);
			}
		}
		else
		{
			std::string cmd = "MODE";
			server.message.sendMessage(c, server.message.getMessage(461, c, cmd));
		}
	}
	else
	{
		if (index +  1 < args.size())
		{
			std::string password = args[++index];
			if (channel->getKey() == password)
			{
				channel->removeKey();
				broadcastModeChange(server, c, channel->getName(), "-k");
			}
			else
			{
				std::string cmd = channel->getName();
				server.message.sendMessage(c, server.message.getMessage(467, c, cmd));
			}
		}
		else 
		{
			std::string cmd = "MODE";
			server.message.sendMessage(c, server.message.getMessage(461, c, cmd));
		}
	}
}

void Mode::handleLimitMode(Channel *channel, bool addMode, Server &server, Client &c, std::vector<std::string> &args, size_t &index)
{
	if (addMode)
	{
		if (index + 1 < args.size())
		{
			size_t limit = std::atoi(args[++index].c_str()); //  o index++;
			channel->setMaxClients(limit);
			broadcastModeChange(server, c, channel->getName(), "+l " + args[index]);
		}
		else
		{
			std::string cmd = "MODE";
			server.message.sendMessage(c, server.message.getMessage(461, c, cmd));
		}
	}
	else
	{
		channel->removeUserLimit();
		broadcastModeChange(server, c, channel->getName(), "-l");
	}

	/*if (addMode)
	{
		if (index + 1 < args.size())
		{
			size_t limit = std::atoi(args[++index].c_str());
			if (limit > 0)
			{
				channel->setMaxClients(limit);
				broadcastModeChange(server, c, channel->getName(), "+l " + args[index]);
			}
			else
			{
				c.sendError(461, c.getNickname(), "MODE", "Invalid limit.");
			}
		}
		else
		{
			c.sendError(461, c.getNickname(), "MODE", "Not enough parameters for +l.");
		}
	}
	else
	{
		channel->removeUserLimit();
		broadcastModeChange(server, c, channel->getName(), "-l");
	}*/
}

void Mode::handleOperatorMode(Channel *channel, bool addMode, Server &server, Client&c, std::vector<std::string> &args, size_t &index)
{
	if (index + 1 < args.size())
	{
		std::string targetUserName = args[++index]; //  o index++
		Client *targetUser = channel->getClientByName(targetUserName);

		if (!targetUser || !channel->isClient(*targetUser))
		{
			std::string cmd = channel->getName();
			server.message.sendMessage(c, server.message.getMessage(401, c, cmd));
		}
		else
		{
			if (addMode)
			{
				channel->addAdmin(*targetUser);
				broadcastModeChange(server, c, channel->getName(), "+o " + targetUserName);
			}
			else
			{
				channel->removeAdmin(*targetUser);
				broadcastModeChange(server, c, channel->getName(), "-o " + targetUserName);
			}
		}
	}
	else
	{
		std::string cmd = "MODE";
		server.message.sendMessage(c, server.message.getMessage(461, c, cmd));
	}    
}

void Mode::broadcastModeChange(Server &server, Client &c, const std::string &channelName, const std::string &modeChange)
{
	Channel *channel = server.getChannel(channelName);
	std::string message = server.getServerName() + " MODE " + channelName + " " + modeChange  + "\r\n";
	//":" + servername + " 324 " + nickname + " " + channel + " " + modes)
	std::string message2 = ":" + server.getServerName() + " 324 " + c.getNickname() + " " + channelName + " " + modeChange + "\r\n";

	/*
	ESTO SE PUEDE ELIMINAR ES SOLO PARA VER LA COMPOSICION DEL RPL_CHANNELMODES
	
	// Si solo se pasa el nombre del canal, mostrar los modos actuales
	if (args.size() == 1)
	{
		std::string channelName = channel->getName();
		std::string modes = channel->getAllModes();

		std::string response = "MODE " + channelName + " " + modes + "\r\n";

		server.sendResponse(c.getFd(), RPL_CHANNELMODES(server.getServerName(), c.getNickname(), channelName, modes));
		std::cout << "Viene del mode excute: " << RPL_CHANNELMODES(server.getServerName(), c.getNickname(), channelName, modes) << std::endl;
		return ;
	}
	*/

	if (channel)
	{
		server.message.sendMessage(c, message2);
		std::cout << "Mensaje enviado al cliente: " << c.getNickname() << std::endl;
		std::cout << "Mensaje envial al canal: " << channelName << std::endl;
	}

	/*Channel *channel = server.getChannel(channelName);

	std::string response = "MODE " + channelName + " " + modeChange + "\r\n";
	channel->broadcastMessage(resonse, c);*/
}