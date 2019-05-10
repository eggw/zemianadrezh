#include "NetworkManagerServer.h"

#include "LoggerNetwork.h"
#include "Server.h"

#include "Tags/PlayerTag.h"
#include "Components/ComponentsPlayer.h"

NetworkManagerServer::NetworkManagerServer(Server& _server)
            : m_server(_server),
              m_listener(),
              m_udpSocket(),
              m_lastRemovedPlayer{} {
	m_udpSocket.setBlocking(false);

	if (m_udpSocket.bind(Packet::Port_UDP_Server) != sf::Socket::Done) {
		LoggerNetwork::get_instance().log(
		  LoggerNetwork::LOG_SENDER::SERVER,
		  LoggerNetwork::LOG_MESSAGE::BIND_PORT_FAILURE);
	}
	else {
		LoggerNetwork::get_instance().log(
		  LoggerNetwork::LOG_SENDER::SERVER,
		  LoggerNetwork::LOG_MESSAGE::BIND_PORT_SUCCESS);
	}

	m_listener.setBlocking(false);
	listen();
}

void NetworkManagerServer::sendPacket(Packet::TCPPacket _type,
                                      sf::TcpSocket*    _recipient,
                                      bool              _exclude) {
	int packetCode = Packet::toInt(_type);

	auto recipients{getTCPRecipients(_recipient, _exclude)};

	PacketSharedPtr packet(new sf::Packet());
	*packet << packetCode;

	switch (_type) {
	//////////////////////////////////////////////////////////////////////////////
	case Packet::TCPPacket::QUIT: {
		*packet << m_lastRemovedPlayer;

		for (const auto& recipient : recipients) {
			PacketSender::get_instance().send(recipient, packet);
		}

		break;
	}
	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	case Packet::TCPPacket::CONNECTIONLOST: {
		for (const auto& recipient : recipients) {
			PacketSender::get_instance().send(recipient, packet);
		}

		break;
	}
	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	case Packet::TCPPacket::DATA_WORLD: {
		encodedChunks worldData = m_server.getWorld().encodeChunks();
		/*
		At the moment, a packet is generated and sent for each chunk
		to each player. In the future, chunks will be sent on an ID basis.
		*/
		for (auto& chunk : worldData) {
			PacketSharedPtr p(new sf::Packet());
			*p << packetCode;
			*p << chunk;
			for (const auto& recipient : recipients) {
				PacketSender::get_instance().send(recipient, p);
			}
		}

		break;
	}
	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	case Packet::TCPPacket::CHAT_MESSAGE: {
		std::string message = m_messages.back().first;
		std::string sender  = m_messages.back().second;
		*packet << message;
		*packet << sender;

		for (const auto& recipient : recipients) {
			PacketSender::get_instance().send(recipient, packet);
		}

		break;
	}
	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	case Packet::TCPPacket::RESPAWN_PLAYER: {
		for (const auto& recipient : recipients) {
			PacketSender::get_instance().send(recipient, packet);
		}

		break;
	}
		//////////////////////////////////////////////////////////////////////////////
	}
}

void NetworkManagerServer::sendPacket(Packet::UDPPacket  _type,
                                      const std::string& _recipientName,
                                      bool               _exclude) {
	int packetCode = Packet::toInt(_type);

	auto recipients{getUDPRecipients(_recipientName, _exclude)};

	PacketSharedPtr packet(new sf::Packet());
	*packet << packetCode;

	switch (_type) {
	//////////////////////////////////////////////////////////////////////////////
	case Packet::UDPPacket::DATA_PLAYER: {
		for (const auto& recipient : recipients) {
			auto view = m_server.m_registry->view<PlayerTag>();
			for (auto& entity : view) {
				const auto name =
				  m_server.m_registry->get<ComponentName>(entity);
				//Perform a quick check to see if the playerdata generated
				//belongs to the recipient's player; if it does, we'll cancel
				//and not redundantly send it back to them
				if (recipient.playerName == name.m_name) {
					continue;
				}
				const auto dir =
				  m_server.m_registry->get<ComponentDirection>(entity);
				const auto vel =
				  m_server.m_registry->get<ComponentPhysics>(entity);
				const auto pos =
				  m_server.m_registry->get<ComponentPosition>(entity);

				ComponentsPlayer data{dir, name, vel, pos};
				*packet << data;

				PacketSender::get_instance().send(
				  &m_udpSocket, packet, recipient.ipAddress, recipient.port);
			}
		}

		break;
	}
		//////////////////////////////////////////////////////////////////////////////
	}
}

void NetworkManagerServer::receiveTCPPackets() {
	int                  packetCode;
	PacketUPtr           packet(new sf::Packet());
	const sf::TcpSocket* toRemove{nullptr};

	for (const auto& connection : m_clientConnections) {
		if (connection->receive(*packet) == sf::Socket::Status::Done) {
			*packet >> packetCode;
			Packet::TCPPacket packetType{Packet::toTCPType(packetCode)};

			switch (packetType) {
			//////////////////////////////////////////////////////////////////////////////
			case Packet::TCPPacket::JUSTJOINED: {
				ComponentsPlayer data;
				sf::Uint16       port;

				*packet >> data;
				*packet >> port;

				m_clientIPs.insert(
				  {connection.get(),
				   {data.compName.m_name, *connection.get(), port}});
				m_server.updatePlayer(data);
				sendPacket(Packet::TCPPacket::DATA_WORLD);
				sendPacket(Packet::TCPPacket::RESPAWN_PLAYER, connection.get());
				sendMessage("Welcome, " + data.compName.m_name + "!", "Server");
				break;
			}
			//////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////////
			case Packet::TCPPacket::QUIT: {
				toRemove = connection.get();
				break;
			}
			//////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////////
			case Packet::TCPPacket::REQUEST_WORLD: {
				sendPacket(Packet::TCPPacket::DATA_WORLD, connection.get());
				break;
			}
			//////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////////
			case Packet::TCPPacket::CHAT_MESSAGE: {
				std::string message;
				std::string sender;
				*packet >> message;
				*packet >> sender;
				m_messages.push_back(std::make_pair(message, sender));

				sendPacket(Packet::TCPPacket::CHAT_MESSAGE);
				break;
			}
				//////////////////////////////////////////////////////////////////////////////

			default:
				break;
			}
		}
	}

	if (toRemove != nullptr) {
		std::string name{m_clientIPs.at(toRemove).playerName};
		removeConnection(toRemove);
		m_server.removePlayer(name);
		sendMessage("Goodbye, " + name + "!", "Server");
		notifyRemoved(name);
	}
}

void NetworkManagerServer::receiveUDPPackets() {
	int            packetCode;
	PacketUPtr     packet(new sf::Packet());
	unsigned short port{Packet::Port_UDP_Server};

	for (auto& client : m_clientIPs) {
		if (m_udpSocket.receive(*packet, client.second.ipAddress, port) ==
		    sf::Socket::Status::Done) {
			*packet >> packetCode;
			Packet::UDPPacket packetType{Packet::toUDPType(packetCode)};

			switch (packetType) {
			//////////////////////////////////////////////////////////////////////////////
			case Packet::UDPPacket::DATA_PLAYER: {
				ComponentsPlayer p;
				*packet >> p;
				//get back to this
				//TODO: figure out how this should work /w local servers
				m_server.updatePlayer(p);

				sendPacket(Packet::UDPPacket::DATA_PLAYER,
				           client.second.playerName);
				break;
			}
				//////////////////////////////////////////////////////////////////////////////
			}
		}
	}
}

void NetworkManagerServer::sendMessage(const std::string& _message,
                                       const std::string& _sender) {
	if (m_messages.size() >= 50) {
		m_messages.clear();
	}

	m_messages.push_back(std::make_pair(_message, _sender));
	sendPacket(Packet::TCPPacket::CHAT_MESSAGE);
}

void NetworkManagerServer::notifyRemoved(const std::string& _name) {
	m_lastRemovedPlayer = _name;
	sendPacket(Packet::TCPPacket::QUIT);
}

void NetworkManagerServer::listen() {
	if (m_listener.listen(Packet::Port_TCP_Server) != sf::Socket::Done) {
		LoggerNetwork::get_instance().log(
		  LoggerNetwork::LOG_SENDER::SERVER,
		  LoggerNetwork::LOG_MESSAGE::LISTEN_PORT_FAILURE);
	}
	else {
		LoggerNetwork::get_instance().log(
		  LoggerNetwork::LOG_SENDER::SERVER,
		  LoggerNetwork::LOG_MESSAGE::LISTEN_PORT_SUCCESS);
	}
}

void NetworkManagerServer::accept() {
	auto socket{std::unique_ptr<sf::TcpSocket>(new sf::TcpSocket())};
	socket->setBlocking(false);

	sf::Socket::Status status = m_listener.accept(*socket);
	if (status == sf::Socket::Done) {
		if (socket->getRemoteAddress() != sf::IpAddress::LocalHost &&
		    !m_server.connectionsAllowed()) {
			LoggerNetwork::get_instance().log(
			  LoggerNetwork::LOG_SENDER::SERVER,
			  LoggerNetwork::LOG_MESSAGE::CONNECTION_BLOCKED);
			return;
		}

		m_clientConnections.push_back(std::move(socket));
		LoggerNetwork::get_instance().log(
		  LoggerNetwork::LOG_SENDER::SERVER,
		  LoggerNetwork::LOG_MESSAGE::CONNECTION_SUCCESS);
	}
}

void NetworkManagerServer::update() {
	PacketSender::get_instance().update();
}

std::vector<sf::TcpSocket*> NetworkManagerServer::getTCPRecipients(
  sf::TcpSocket* _recipient,
  bool           _exclude) {
	std::vector<sf::TcpSocket*> recipients{};

	if (_recipient == nullptr) {
		for (const auto& client : m_clientConnections) {
			recipients.push_back(client.get());
		}
	}
	else {
		if (!_exclude) {
			recipients.push_back(_recipient);
		}
		else {
			for (const auto& client : m_clientConnections) {
				if (client.get() != _recipient) {
					recipients.push_back(client.get());
				}
			}
		}
	}

	return recipients;
}

std::vector<NetworkManagerServer::IPInfo>
NetworkManagerServer::getUDPRecipients(const std::string& _recipientName,
                                       bool               _exclude) {
	std::vector<IPInfo> recipients{};

	if (_recipientName == "") {
		for (const auto& client : m_clientIPs) {
			recipients.push_back(client.second);
		}
	}
	else {
		if (!_exclude) {
			for (const auto& client : m_clientIPs) {
				if (client.second.playerName == _recipientName) {
					recipients.push_back(client.second);
				}
			}
		}
		else {
			for (const auto& client : m_clientIPs) {
				if (client.second.playerName != _recipientName) {
					recipients.push_back(client.second);
				}
			}
		}
	}

	return recipients;
}

void NetworkManagerServer::removeConnection(const sf::TcpSocket* _con) {
	m_clientConnections.erase(std::remove_if(m_clientConnections.begin(),
	                                         m_clientConnections.end(),
	                                         [&](const auto& con) {
		                                         //return true to have it removed
		                                         return (con.get() == _con);
	                                         }),
	                          m_clientConnections.end());

	for (auto it{m_clientIPs.begin()}; it != m_clientIPs.end();) {
		if (it->first == _con) {
			it = m_clientIPs.erase(it);
			return;
		}
		else {
			++it;
		}
	}
}
