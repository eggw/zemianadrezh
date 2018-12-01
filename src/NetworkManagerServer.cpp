#include "NetworkManagerServer.h"

#include "LoggerNetwork.h"
#include <iostream>
#include "Server.h"

NetworkManagerServer::NetworkManagerServer(Server& _server)
	: m_server(_server),
	  m_listener(),
	  m_udpSocket(){

	m_udpSocket.setBlocking(false);

	if (m_udpSocket.bind(Packet::Port_UDP_Server) != sf::Socket::Done) {
		LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::SERVER,
		LoggerNetwork::LOG_MESSAGE::BIND_PORT_FAILURE);
		std::cout << "NetworkManagerServer: m_udpSocket bound to " << m_udpSocket.getLocalPort() << std::endl;
	}
	else {
		LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::SERVER,
		LoggerNetwork::LOG_MESSAGE::BIND_PORT_SUCCESS);
	}

	m_listener.setBlocking(false);
	listen();
}

void NetworkManagerServer::sendPacket(Packet::TCPPacket _type, sf::TcpSocket* _recipient, bool _exclude) {
	int packetCode = Packet::toInt(_type);

	std::vector<sf::TcpSocket*> recipients;

	if(_recipient == nullptr) {
		for(auto& client : m_clientConnections) {
			recipients.push_back(client.get());
		}
	}
	else {
		if(!_exclude) {
			recipients.push_back(_recipient);
		}
		else {
			for(auto& client : m_clientConnections) {
				if(client.get() != _recipient) {
					recipients.push_back(client.get());
				}
			}
		}
	}

	PacketSharedPtr packet(new sf::Packet());
	*packet << packetCode;

	switch(_type) {
	//////////////////////////////////////////////////////////////////////////////
	case Packet::TCPPacket::DATA_WORLD: {

			World::EncodedWorldData worldData = m_server.getWorld().encodeData();

			for(auto& recipient : recipients) {
				*packet << worldData.chunkIDs;
				*packet << worldData.invisibleBlocks;
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

			for(auto& recipient : recipients) {
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
			for(auto& recipient : recipients) {
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
										sf::IpAddress _recipient,
										bool _exclude) {

	int packetCode = Packet::toInt(_type);

	std::vector<IPInfo> recipients;

	if (_recipient == sf::IpAddress::None) {
		for (auto& client : m_clientIPs) {
			recipients.push_back(client.second);
		}
	}
	else {
		if (!_exclude) {
			for (auto& client : m_clientIPs) {
				if (client.second.ipAddress == _recipient) {
					recipients.push_back(client.second);
				}
			}
		}
		else {
			for (auto& client : m_clientIPs) {
				if (client.second.ipAddress != _recipient) {
					recipients.push_back(client.second);
				}
			}
		}
	}

	PacketSharedPtr packet(new sf::Packet());
	*packet << packetCode;

	switch (_type) {
	//////////////////////////////////////////////////////////////////////////////
	case Packet::UDPPacket::DATA_PLAYER: {

		for (auto& recipient : recipients) {
			for (auto& player : *m_server.getOtherPlayers()) {
				Player::EncodedPlayerData playerData = player->encodeData();

				*packet << playerData.playerName;

				/*
				NOTE: this doesn't work atm because I cannot differentiate
				between local as in same process, or local as in connected
				to LocalHost. I've commented it out for now.

				//Here we do a quick check to see if the playerdata generated
				//belongs to the recipient's player; if it does, we'll cancel
				//and not redundantly send it back to them
				auto i = m_clientIPs.find(playerData.playerName);
				if (i != m_clientIPs.end()) {
					if (i->second == recipient) {
						continue;
					}
				}
				*/

				*packet << playerData.facingLeft;
				*packet << playerData.velocityX;
				*packet << playerData.velocityY;
				*packet << playerData.positionX;
				*packet << playerData.positionY;
	
				PacketSender::get_instance().send(&m_udpSocket, packet, recipient.ipAddress, recipient.port);
				LoggerNetwork::get_instance().logConsole(LoggerNetwork::LOG_SENDER::SERVER,
					LoggerNetwork::LOG_PACKET_DATATRANSFER::PACKET_SENT,
					packetCode);
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

	for(auto& connection : m_clientConnections) {
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

					m_clientIPs.insert({ playerData.playerName, {connection.get()->getRemoteAddress(), port}});
					m_server.addPlayer(playerData);
					sendPacket(Packet::TCPPacket::DATA_WORLD);
					sendPacket(Packet::TCPPacket::RESPAWN_PLAYER, connection.get());
					sendMessage("Welcome, " + playerData.playerName + "!", "Server");
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

				*packet >> playerData.playerName;
				*packet >> playerData.facingLeft;
				*packet >> playerData.velocityX;
				*packet >> playerData.velocityY;
				*packet >> playerData.positionX;
				*packet >> playerData.positionY;

				m_server.updateOtherPlayers(playerData);

				sendPacket(Packet::UDPPacket::DATA_PLAYER, client.second.ipAddress);
				break;
				}
			//////////////////////////////////////////////////////////////////////////////
			}
		}
	}
}

void NetworkManagerServer::sendMessage(std::string _message, std::string _sender) {
	if(m_messages.size() == 50) {
		m_messages.clear();
	}

	m_messages.push_back(std::make_pair(_message, _sender));
	sendPacket(Packet::TCPPacket::CHAT_MESSAGE);
}

const std::vector<std::unique_ptr<sf::TcpSocket>>& NetworkManagerServer::getClients() {
	return m_clientConnections;
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
	m_clientConnections.push_back(std::unique_ptr<sf::TcpSocket>(new sf::TcpSocket()));
	m_clientConnections.back()->setBlocking(false);

	sf::Socket::Status status = m_listener.accept(*m_clientConnections.back());
	if(status != sf::Socket::Done) {
		//LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::SERVER,
		//LoggerNetwork::LOG_MESSAGE::CONNECTION_FAILURE);
		m_clientConnections.pop_back();
	}
	else {
		if (m_clientConnections.back()->getRemoteAddress() != sf::IpAddress::LocalHost
				&&
				!m_server.connectionsAllowed()) {
			LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::SERVER,
											  LoggerNetwork::LOG_MESSAGE::CONNECTION_BLOCKED);
			m_clientConnections.pop_back();
			return;
		}

		LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::SERVER,
										  LoggerNetwork::LOG_MESSAGE::CONNECTION_SUCCESS);
	}
}

void NetworkManagerServer::update() {
	PacketSender::get_instance().update();
}
