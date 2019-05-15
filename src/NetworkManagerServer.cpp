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

	using std::placeholders::_1;
	using std::placeholders::_2;

	m_TCPSender.add(Packet::TCP::QUIT, std::bind(&NetworkManagerServer::sendQuit, this, _1));
	m_TCPSender.add(Packet::TCP::DATA_WORLD, std::bind(&NetworkManagerServer::sendDataWorld, this, _1));
	m_TCPSender.add(Packet::TCP::CHAT_MESSAGE, std::bind(&NetworkManagerServer::sendChatMessage, this, _1));

	m_TCPReceiver.add(Packet::TCP::JUST_JOINED, std::bind(&NetworkManagerServer::receiveJustJoined, this, _1, _2));
	m_TCPReceiver.add(Packet::TCP::QUIT, std::bind(&NetworkManagerServer::receiveQuit, this, _1, _2));
	m_TCPReceiver.add(Packet::TCP::REQUEST_WORLD, std::bind(&NetworkManagerServer::receiveRequestWorld, this, _1, _2));
	m_TCPReceiver.add(Packet::TCP::CHAT_MESSAGE, std::bind(&NetworkManagerServer::receiveChatMessage, this, _1, _2));

	m_UDPSender.add(Packet::UDP::DATA_PLAYER, std::bind(&NetworkManagerServer::sendDataPlayer, this, _1));
	m_UDPReceiver.add(Packet::UDP::DATA_PLAYER, std::bind(&NetworkManagerServer::receiveDataPlayer, this, _1, _2));

	m_listener.setBlocking(false);
	listen();
}

void NetworkManagerServer::sendPacket(Packet::TCP _type,
                                      sf::TcpSocket*    _recipient,
                                      bool              _exclude) {
	int code = Packet::toInt(_type);

	auto recipients{getTCPRecipients(_recipient, _exclude)};

	PacketSharedPtr p(new sf::Packet());
	*p << code;

	std::vector<PacketSharedPtr> packets;
	packets.push_back(std::move(p));

	m_TCPSender.call(_type, packets);

	for (const auto& recipient : recipients) {
		for (auto& packet : packets) {
			PacketSender::get_instance().send(recipient, packet);
		}
	}
}

void NetworkManagerServer::sendPacket(Packet::UDP  _type,
                                      const std::string& _recipientName,
                                      bool               _exclude) {
	int code = Packet::toInt(_type);

	auto recipients{getUDPRecipients(_recipientName, _exclude)};

	PacketSharedPtr p(new sf::Packet());
	*p << code;

	std::vector<PacketSharedPtr> packets;
	packets.push_back(std::move(p));

	m_UDPSender.call(_type, packets);

	for (const auto& recipient : recipients) {
		for(auto& packet : packets) {
			PacketSender::get_instance().send(&m_udpSocket, packet, recipient.ipAddress, recipient.port);
		}
	}
}

void NetworkManagerServer::receiveTCPPackets() {
	int                  code;
	PacketUPtr           packet(new sf::Packet());

	for (const auto& connection : m_clientConnections) {
		if (connection->receive(*packet) == sf::Socket::Status::Done) {
			*packet >> code;
			Packet::TCP packetType{Packet::toTCPType(code)};

			m_TCPReceiver.call(packetType, packet.get(), connection.get());
		}
	}
}

void NetworkManagerServer::receiveUDPPackets() {
	int            code;
	PacketUPtr     packet(new sf::Packet());
	unsigned short port{Packet::Port_UDP_Server};

	for (auto& client : m_clientIPs) {
		if (m_udpSocket.receive(*packet, client.second.ipAddress, port) ==
		    sf::Socket::Status::Done) {
			*packet >> code;
			Packet::UDP packetType{Packet::toUDPType(code)};

			m_UDPReceiver.call(packetType, packet.get(), client.first);
		}
	}
}

void NetworkManagerServer::sendMessage(const Message& _msg) {
	if (m_messages.size() >= 50) {
		m_messages.clear();
	}

	m_messages.push_back({_msg});
	sendPacket(Packet::TCP::CHAT_MESSAGE);
}

void NetworkManagerServer::removePlayer(sf::TcpSocket* _connection) {
	if (_connection == nullptr) {
		return;
	}

	std::string name{m_clientIPs.at(_connection).playerName};
	removeConnection(_connection);
	m_server.removePlayer(name);
	sendMessage({"Server", "Goodbye, " + name + "!"});
	m_lastRemovedPlayer = name;
	sendPacket(Packet::TCP::QUIT);
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

size_t NetworkManagerServer::connectedPlayers() const {
	return m_clientConnections.size();
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

void NetworkManagerServer::removeConnection(sf::TcpSocket* _con) {
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

void NetworkManagerServer::sendQuit(std::vector<PacketSharedPtr>& _p) {
	*_p[0] << m_lastRemovedPlayer;
}

void NetworkManagerServer::sendDataWorld(std::vector<PacketSharedPtr>& _p) {
	auto worldData = m_server.encodeWorldChunks();

	/*At the moment, a packet is generated and sent for each chunk
	to each player. In the future, chunks will be sent on an ID basis.*/

	int index{0};
	for (auto& chunk : worldData) {
		PacketSharedPtr p(new sf::Packet());
		*p << Packet::toInt(Packet::TCP::DATA_WORLD);
		*p << chunk;
		_p.resize(index + 1);
		_p[index] = std::move(p);
		++index;
	}
}

void NetworkManagerServer::sendChatMessage(std::vector<PacketSharedPtr>& _p) {
	Message msg = m_messages.back();
	*_p[0] << msg.sender;
	*_p[0] << msg.content;
}


void NetworkManagerServer::receiveJustJoined(sf::Packet* _p, sf::TcpSocket* _conn) {

	ComponentsPlayer data;
	sf::Uint16       port;

	*_p >> data;
	*_p >> port;

	m_clientIPs.insert(
	  {_conn,
	   {data.compName.m_name, *_conn, port}});
	m_server.updatePlayer(data);
	sendPacket(Packet::TCP::DATA_WORLD);
	sendPacket(Packet::TCP::RESPAWN_PLAYER, _conn);
	sendMessage(
	  {"Server", "Welcome, " + data.compName.m_name + "!"});

}

void NetworkManagerServer::receiveQuit(sf::Packet* _p, sf::TcpSocket* _conn) {
	removePlayer(_conn);
}

void NetworkManagerServer::receiveRequestWorld(sf::Packet* _p, sf::TcpSocket* _conn) {
	sendPacket(Packet::TCP::DATA_WORLD, _conn);
}

void NetworkManagerServer::receiveChatMessage(sf::Packet* _p, sf::TcpSocket* _conn) {
	std::string message;
	std::string sender;
	*_p >> message;
	*_p >> sender;
	m_messages.push_back({message, sender});

	sendPacket(Packet::TCP::CHAT_MESSAGE);
}


void NetworkManagerServer::sendDataPlayer(std::vector<PacketSharedPtr>& _p) {
	int index{0};

	auto view = m_server.m_registry->view<PlayerTag>();
	for (auto& entity : view) {
		const auto name =
		  m_server.m_registry->get<ComponentName>(entity);

		const auto dir =
		  m_server.m_registry->get<ComponentDirection>(entity);
		const auto vel =
		  m_server.m_registry->get<ComponentPhysics>(entity);
		const auto pos =
		  m_server.m_registry->get<ComponentPosition>(entity);

		ComponentsPlayer data{dir, name, vel, pos};

		PacketSharedPtr p(new sf::Packet());
		*p << Packet::toInt(Packet::UDP::DATA_PLAYER);
		*p << data;
		_p.resize(index + 1);
		_p[index] = std::move(p);
		++index;
	}

}

void NetworkManagerServer::receiveDataPlayer(sf::Packet* _p, sf::TcpSocket* _conn) {
	ComponentsPlayer p;
	*_p >> p;
	m_server.updatePlayer(p);

	sendPacket(Packet::UDP::DATA_PLAYER,
			   p.compName.m_name);
}
