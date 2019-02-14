#include "NetworkManagerServer.h"

#include <iostream>

#include "LoggerNetwork.h"
#include "Server.h"

NetworkManagerServer::NetworkManagerServer(Server& _server)
	: m_server(_server),
	  m_listener(),
	  m_udpSocket(),
	  m_lastRemovedPlayer{}{

	m_udpSocket.setBlocking(false);

	if (m_udpSocket.bind(Packet::Port_UDP_Server) != sf::Socket::Done) {
		LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::SERVER,
		LoggerNetwork::LOG_MESSAGE::BIND_PORT_FAILURE);
	}
	else {
		LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::SERVER,
		LoggerNetwork::LOG_MESSAGE::BIND_PORT_SUCCESS);
	}

	m_listener.setBlocking(false);
	listen();
}

void NetworkManagerServer::sendPacket(Packet::TCPPacket _type,
									  sf::TcpSocket* _recipient,
									  bool _exclude) {
	int packetCode = Packet::toInt(_type);

	auto recipients{ getTCPRecipients(_recipient, _exclude) };

	PacketSharedPtr packet(new sf::Packet());
	*packet << packetCode;

	switch(_type) {
	//////////////////////////////////////////////////////////////////////////////
	case Packet::TCPPacket::QUIT: {
			*packet << m_lastRemovedPlayer;

			for(const auto& recipient : recipients) {
				PacketSender::get_instance().send(recipient, packet);
			}

			LoggerNetwork::get_instance().logConsole(LoggerNetwork::LOG_SENDER::SERVER,
					LoggerNetwork::LOG_PACKET_DATATRANSFER::PACKET_SENT,
					packetCode);

			break;
		}
    //////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
	case Packet::TCPPacket::CONNECTIONLOST: {
			for(const auto& recipient : recipients) {
				PacketSender::get_instance().send(recipient, packet);
			}

			LoggerNetwork::get_instance().logConsole(LoggerNetwork::LOG_SENDER::SERVER,
					LoggerNetwork::LOG_PACKET_DATATRANSFER::PACKET_SENT,
					packetCode);

			break;
		}
    //////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	case Packet::TCPPacket::DATA_WORLD: {

			World::EncodedWorldData worldData = m_server.getWorld().encodeData();

			for(const auto& recipient : recipients) {
				*packet << worldData;

				PacketSender::get_instance().send(recipient, packet);
				LoggerNetwork::get_instance().logConsole(LoggerNetwork::LOG_SENDER::SERVER,
						LoggerNetwork::LOG_PACKET_DATATRANSFER::PACKET_SENT,
						packetCode);

			}

			break;
		}
	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	case Packet::TCPPacket::CHAT_MESSAGE: {
			std::string message = m_messages.back().first;
			std::string sender = m_messages.back().second;
			*packet << message;
			*packet << sender;

			for(const auto& recipient : recipients) {
				PacketSender::get_instance().send(recipient, packet);
			}

			LoggerNetwork::get_instance().logConsole(LoggerNetwork::LOG_SENDER::SERVER,
					LoggerNetwork::LOG_PACKET_DATATRANSFER::PACKET_SENT,
					packetCode);

			break;
		}
    //////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
	case Packet::TCPPacket::RESPAWN_PLAYER: {
			for(const auto& recipient : recipients) {
				PacketSender::get_instance().send(recipient, packet);
				LoggerNetwork::get_instance().logConsole(LoggerNetwork::LOG_SENDER::SERVER,
						LoggerNetwork::LOG_PACKET_DATATRANSFER::PACKET_SENT,
						packetCode);

			}

			break;
		}
	//////////////////////////////////////////////////////////////////////////////
	}
}

void NetworkManagerServer::sendPacket(	Packet::UDPPacket _type,
										const sf::IpAddress& _recipient,
										bool _exclude) {

	int packetCode = Packet::toInt(_type);

	auto recipients{ getUDPRecipients(_recipient, _exclude) };

	PacketSharedPtr packet(new sf::Packet());
	*packet << packetCode;

	switch (_type) {
	//////////////////////////////////////////////////////////////////////////////
	case Packet::UDPPacket::DATA_PLAYER: {

		for (const auto& recipient : recipients) {
			for (const auto& player : *m_server.getPlayers()) {
				Player::EncodedPlayerData playerData = player->encodeData();

				*packet << playerData;

				//Here we do a quick check to see if the playerdata generated
				//belongs to the recipient's player; if it does, we'll cancel
				//and not redundantly send it back to them
				//TODO: confirm this works!

				if(recipient.playerName != playerData.playerName) {
					PacketSender::get_instance().send(&m_udpSocket, packet, recipient.ipAddress, recipient.port);
					LoggerNetwork::get_instance().logConsole(LoggerNetwork::LOG_SENDER::SERVER,
					LoggerNetwork::LOG_PACKET_DATATRANSFER::PACKET_SENT,
					packetCode);
				}
			}
		}

		break;
	}
	//////////////////////////////////////////////////////////////////////////////
	}

}

void NetworkManagerServer::receiveTCPPackets() {
	int packetCode;
	PacketUPtr packet(new sf::Packet());
	const sf::TcpSocket* toRemove{ nullptr };

	for(const auto& connection : m_clientConnections) {
		if (connection->receive(*packet) == sf::Socket::Status::Done) {
			*packet >> packetCode;
			Packet::TCPPacket packetType{Packet::toTCPType(packetCode)};
			LoggerNetwork::get_instance().logConsole(LoggerNetwork::LOG_SENDER::SERVER,
					LoggerNetwork::LOG_PACKET_DATATRANSFER::PACKET_RECEIVED,
					packetCode);

			switch(packetType) {

			//////////////////////////////////////////////////////////////////////////////
			case Packet::TCPPacket::JUSTJOINED: {
					Player::EncodedPlayerData playerData = Player::EncodedPlayerData();
					sf::Uint16 port;
					*packet >> playerData.playerName;
					*packet >> port;

					m_clientIPs.insert({ connection.get(), {playerData.playerName, *connection.get(), port}});
					m_server.addPlayer(playerData);
					sendPacket(Packet::TCPPacket::DATA_WORLD);
					sendPacket(Packet::TCPPacket::RESPAWN_PLAYER, connection.get());
					sendMessage("Welcome, " + playerData.playerName + "!", "Server");
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

	if(toRemove != nullptr){
		std::string name { m_clientIPs.at(toRemove).playerName };
        removeConnection(toRemove);
        sendMessage("Goodbye, " + name + "!", "Server");
        notifyRemoved(name);
	}
}

void NetworkManagerServer::receiveUDPPackets() {
	int packetCode;
	PacketUPtr packet(new sf::Packet());
	unsigned short port{ Packet::Port_UDP_Server };

	for (auto& client : m_clientIPs) {
		if (m_udpSocket.receive(*packet, client.second.ipAddress, port) == sf::Socket::Status::Done) {
			*packet >> packetCode;
			Packet::UDPPacket packetType{ Packet::toUDPType(packetCode) };
			LoggerNetwork::get_instance().logConsole(LoggerNetwork::LOG_SENDER::SERVER,
				LoggerNetwork::LOG_PACKET_DATATRANSFER::PACKET_RECEIVED,
				packetCode);

			switch (packetType) {
			//////////////////////////////////////////////////////////////////////////////
			case Packet::UDPPacket::DATA_PLAYER: {
				Player::EncodedPlayerData playerData;

				*packet >> playerData;

				m_server.updatePlayer(playerData);

				sendPacket(Packet::UDPPacket::DATA_PLAYER, client.second.ipAddress);
				break;
				}
			//////////////////////////////////////////////////////////////////////////////
			}
		}
	}
}

void NetworkManagerServer::sendMessage(const std::string& _message,
									   const std::string& _sender) {
	if(m_messages.size() >= 50) {
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
	if(m_listener.listen(Packet::Port_TCP_Server) != sf::Socket::Done) {
		LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::SERVER,
										  LoggerNetwork::LOG_MESSAGE::LISTEN_PORT_FAILURE);
	} else {
		LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::SERVER,
										  LoggerNetwork::LOG_MESSAGE::LISTEN_PORT_SUCCESS);
	}
}

void NetworkManagerServer::accept() {
	auto socket{ std::unique_ptr<sf::TcpSocket>(new sf::TcpSocket()) };
	socket->setBlocking(false);

	sf::Socket::Status status = m_listener.accept(*socket);
	if (status == sf::Socket::Done) {
		if (socket->getRemoteAddress() != sf::IpAddress::LocalHost
				&&
				!m_server.connectionsAllowed()) {
			LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::SERVER,
											  LoggerNetwork::LOG_MESSAGE::CONNECTION_BLOCKED);
			return;
		}

		m_clientConnections.push_back(std::move(socket));
		LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::SERVER,
										  LoggerNetwork::LOG_MESSAGE::CONNECTION_SUCCESS);
	}
}

void NetworkManagerServer::update() {
	PacketSender::get_instance().update();
}

std::vector<sf::TcpSocket*> NetworkManagerServer::getTCPRecipients(sf::TcpSocket* _recipient, bool _exclude) {
    std::vector<sf::TcpSocket*> recipients{};

    if(_recipient == nullptr) {
		for(const auto& client : m_clientConnections) {
			recipients.push_back(client.get());
		}
	}
	else {
		if(!_exclude) {
			recipients.push_back(_recipient);
		}
		else {
			for(const auto& client : m_clientConnections) {
				if(client.get() != _recipient) {
					recipients.push_back(client.get());
				}
			}
		}
	}

	return recipients;
}

std::vector<NetworkManagerServer::IPInfo> NetworkManagerServer::getUDPRecipients(sf::IpAddress _recipient, bool _exclude) {
	std::vector<IPInfo> recipients{};

	if (_recipient == sf::IpAddress::None) {
		for (const auto& client : m_clientIPs) {
			recipients.push_back(client.second);
		}
	}
	else {
		if (!_exclude) {
			for (const auto& client : m_clientIPs) {
				if (client.second.ipAddress == _recipient) {
					recipients.push_back(client.second);
				}
			}
		}
		else {
			for (const auto& client : m_clientIPs) {
				if (client.second.ipAddress != _recipient) {
					recipients.push_back(client.second);
				}
			}
		}
	}

	return recipients;
}

void NetworkManagerServer::removeConnection(const sf::TcpSocket* _con) {
    m_clientConnections.erase(std::remove_if(
						   m_clientConnections.begin(),
						   m_clientConnections.end(),
	[&](const auto& con) {
		//return true to have it removed
		return (con.get() == _con);
	}), m_clientConnections.end());


	for(auto it{ m_clientIPs.begin() }; it != m_clientIPs.end(); ) {
		if(it->first == _con) {
			it = m_clientIPs.erase(it);
			return;
		}
		else{
			++it;
		}
    }
}


