#include "NetworkManagerServer.h"

#include "LoggerNetwork.h"
#include "Server.h"

#include "Tags/PlayerTag.h"
#include "Components/ComponentsPlayer.h"

NetworkManagerServer::NetworkManagerServer(Server& _server)
            : m_server(_server),
              m_listener(),
              m_udpSocket(),
              m_lastRemovedPlayer{},
              m_lastTime{m_server.m_world->getTime()},
              m_connectionsToRemove{} {
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

	m_TCPSender.add(Packet::TCP::QUIT,
	                std::bind(&NetworkManagerServer::sendQuit, this, _1, _2));
	m_TCPSender.add(
	  Packet::TCP::DATA_WORLD,
	  std::bind(&NetworkManagerServer::sendDataWorld, this, _1, _2));
	m_TCPSender.add(
	  Packet::TCP::CHAT_MESSAGE,
	  std::bind(&NetworkManagerServer::sendChatMessage, this, _1, _2));

	m_TCPReceiver.add(
	  Packet::TCP::JUST_JOINED,
	  std::bind(&NetworkManagerServer::receiveJustJoined, this, _1, _2));
	m_TCPReceiver.add(
	  Packet::TCP::QUIT,
	  std::bind(&NetworkManagerServer::receiveQuit, this, _1, _2));
	m_TCPReceiver.add(
	  Packet::TCP::REQUEST_WORLD,
	  std::bind(&NetworkManagerServer::receiveRequestWorld, this, _1, _2));
	m_TCPReceiver.add(
	  Packet::TCP::CHAT_MESSAGE,
	  std::bind(&NetworkManagerServer::receiveChatMessage, this, _1, _2));

	m_UDPSender.add(
	  Packet::UDP::DATA_PLAYER,
	  std::bind(&NetworkManagerServer::sendDataPlayer, this, _1, _2));
	m_UDPSender.add(
	  Packet::UDP::DATA_WORLDTIME,
	  std::bind(&NetworkManagerServer::sendDataWorldTime, this, _1, _2));
	m_UDPReceiver.add(
	  Packet::UDP::DATA_PLAYER,
	  std::bind(&NetworkManagerServer::receiveDataPlayer, this, _1, _2));

	m_listener.setBlocking(false);
	listen();
}

void NetworkManagerServer::sendPacket(Packet::TCP    _type,
                                      sf::TcpSocket* _recipient,
                                      bool           _exclude) {
	int code = Packet::toInt(_type);

	auto recipients{getRecipients(_recipient, _exclude)};

	for (const auto& recipient : recipients) {
		PacketSharedPtr p(new sf::Packet());
		*p << code;
		PacketSharedPtrs packets;
		packets.push_back(std::move(p));

		m_TCPSender.call(_type, packets, recipient.socket);

		for (auto& packet : packets) {
			PacketSender::get_instance().send(recipient.socket, packet);
		}
	}
}

void NetworkManagerServer::sendPacket(Packet::UDP    _type,
                                      sf::TcpSocket* _recipient,
                                      bool           _exclude) {
	int  code{Packet::toInt(_type)};
	auto recipients{getRecipients(_recipient, _exclude)};

	for (const auto& recipient : recipients) {
		PacketSharedPtr p(new sf::Packet());
		*p << code;
		PacketSharedPtrs packets;
		packets.push_back(std::move(p));

		m_UDPSender.call(_type, packets, recipient.socket);

		for (auto& packet : packets) {
			PacketSender::get_instance().send(&m_udpSocket,
			                                  packet,
			                                  recipient.ipInfo->ipAddress,
			                                  recipient.ipInfo->port);
		}
	}
}

void NetworkManagerServer::receiveTCPPackets() {
	int        code;
	PacketUPtr packet{new sf::Packet()};

	for (const auto& connection : m_clientIPs) {
		if (connection.first->receive(*packet) == sf::Socket::Status::Done) {
			*packet >> code;
			Packet::TCP packetType{Packet::toTCPType(code)};

			m_TCPReceiver.call(
			  packetType, packet.get(), connection.first.get());
		}
	}

	//Removes from m_clientIPs any clients that may have been
	//marked for removal through receiving a QUIT packet
	removeClients();
}

void NetworkManagerServer::receiveUDPPackets() {
	int            code;
	PacketUPtr     packet{new sf::Packet()};
	unsigned short port{Packet::Port_UDP_Server};

	for (auto& client : m_clientIPs) {
		//don't receive UDP packets from connections with uninitialised IPInfos
		//(IPInfos are initialised upon receiving a JUSTJOINED packet)
		if (client.second == nullptr) {
			continue;
		}

		if (m_udpSocket.receive(*packet, client.second->ipAddress, port) ==
		    sf::Socket::Status::Done) {
			*packet >> code;
			Packet::UDP packetType{Packet::toUDPType(code)};

			m_UDPReceiver.call(packetType, packet.get(), client.first.get());
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

void NetworkManagerServer::sendWorldTime() {
	auto currentTime{m_server.m_world->getTime()};
	if(m_lastTime == currentTime) {
		return;
	}
	sendPacket(Packet::UDP::DATA_WORLDTIME);
	m_lastTime = currentTime;
}

void NetworkManagerServer::markClientForRemoval(sf::TcpSocket* _conn) {
	auto info{getIPInfo(_conn)};

	if (info == nullptr) {
		return;
	}
	
	m_lastRemovedPlayer = info->playerName;
	m_server.removePlayer(m_lastRemovedPlayer);
	sendMessage({"Server", "Goodbye, " + info->playerName + "!"});
	sendPacket(Packet::TCP::QUIT, _conn, true);

	m_connectionsToRemove.push_back(_conn);
}

//Remove all connections that were marked for removal by
//removePlayer(). called after we receive TCP packets to
//not interfere with our iteration
void NetworkManagerServer::removeClients() {
	if (m_connectionsToRemove.empty()) {
		return;
	}

	auto it{std::begin(m_clientIPs)};

	while (it != std::end(m_clientIPs)) {
		auto result{std::find(std::begin(m_connectionsToRemove),
		                      std::end(m_connectionsToRemove),
		                      it->first.get())};
		if (result != std::end(m_connectionsToRemove)) {
			it = m_clientIPs.erase(it);
		}
		else {
			++it;
		}
	}

	m_connectionsToRemove.clear();
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

		m_clientIPs.insert(std::make_pair(std::move(socket), nullptr));
		LoggerNetwork::get_instance().log(
		  LoggerNetwork::LOG_SENDER::SERVER,
		  LoggerNetwork::LOG_MESSAGE::CONNECTION_SUCCESS);
	}
}

void NetworkManagerServer::update() {
	sendWorldTime();
	PacketSender::get_instance().update();
}

size_t NetworkManagerServer::connectedPlayers() const {
	return m_clientIPs.size();
}

std::vector<NetworkManagerServer::ConnectionData>
NetworkManagerServer::getRecipients(sf::TcpSocket* _recipient, bool _exclude) {
	std::vector<ConnectionData> recipients{};

	//A null recipient means we should send this to all connected players.
	if (_recipient == nullptr) {
		for (const auto& client : m_clientIPs) {
			//We don't want to send anything to clients that
			//haven't had their IPInfo initialised yet
			if (client.second != nullptr) {
				recipients.push_back({client.first.get(), client.second.get()});
			}
		}

		return recipients;
	}

	if (!_exclude) {
		for (const auto& client : m_clientIPs) {
			if (client.first.get() == _recipient && client.second != nullptr) {
				recipients.push_back({client.first.get(), client.second.get()});
			}
		}

		return recipients;
	}

	for (const auto& client : m_clientIPs) {
		if (client.first.get() != _recipient && client.second != nullptr) {
			recipients.push_back({client.first.get(), client.second.get()});
		}
	}

	return recipients;
}

NetworkManagerServer::IPInfo* NetworkManagerServer::getIPInfo(
  sf::TcpSocket* _conn) const {
	auto it{m_clientIPs.find(_conn)};
	if (it != m_clientIPs.end()) {
		return it->second.get();
	}
	return nullptr;
}

void NetworkManagerServer::sendQuit(PacketSharedPtrs& _p,
                                    sf::TcpSocket*   _conn) {
	*_p[0] << m_lastRemovedPlayer;
}

void NetworkManagerServer::sendDataWorld(PacketSharedPtrs& _p,
                                         sf::TcpSocket*    _conn) {
	_p.clear();
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

void NetworkManagerServer::sendChatMessage(PacketSharedPtrs& _p,
                                           sf::TcpSocket*    _conn) {
	Message msg = m_messages.back();
	*_p[0] << msg.sender;
	*_p[0] << msg.content;
}

void NetworkManagerServer::receiveJustJoined(sf::Packet*    _p,
                                             sf::TcpSocket* _conn) {
	ComponentsPlayer data;
	sf::Uint16       port;

	*_p >> data;
	*_p >> port;

	auto it{m_clientIPs.find(_conn)};
	if (it != m_clientIPs.end()) {
		it->second =
		  std::make_unique<IPInfo>(data.compName.m_name, *_conn, port);
	}

	m_server.updatePlayer(data);
	sendPacket(Packet::TCP::DATA_WORLD);
	sendPacket(Packet::TCP::RESPAWN_PLAYER, _conn);
	sendMessage({"Server", "Welcome, " + data.compName.m_name + "!"});
}

void NetworkManagerServer::receiveQuit(sf::Packet* _p, sf::TcpSocket* _conn) {
	markClientForRemoval(_conn);
}

void NetworkManagerServer::receiveRequestWorld(sf::Packet*    _p,
                                               sf::TcpSocket* _conn) {
	sendPacket(Packet::TCP::DATA_WORLD, _conn);
}

void NetworkManagerServer::receiveChatMessage(sf::Packet*    _p,
                                              sf::TcpSocket* _conn) {
	std::string message;
	std::string sender;
	*_p >> message;
	*_p >> sender;
	m_messages.push_back({message, sender});

	sendPacket(Packet::TCP::CHAT_MESSAGE);
}

void NetworkManagerServer::sendDataPlayer(PacketSharedPtrs& _p,
                                          sf::TcpSocket*    _conn) {
	_p.clear();
	int  index{0};
	auto recipientInfo{getIPInfo(_conn)};

	auto view = m_server.m_registry->view<PlayerTag>();
	for (auto& entity : view) {
		const auto name = m_server.m_registry->get<ComponentName>(entity);

		//Don't send a client's own player data back to it!
		if (name.m_name == recipientInfo->playerName) {
			continue;
		}

		const auto dir = m_server.m_registry->get<ComponentDirection>(entity);
		const auto vel = m_server.m_registry->get<ComponentPhysics>(entity);
		const auto pos = m_server.m_registry->get<ComponentPosition>(entity);

		ComponentsPlayer data{dir, name, vel, pos};

		PacketSharedPtr p(new sf::Packet());
		*p << Packet::toInt(Packet::UDP::DATA_PLAYER);
		*p << data;
		_p.resize(index + 1);
		_p[index] = std::move(p);
		++index;
	}
}

void NetworkManagerServer::sendDataWorldTime(PacketSharedPtrs& _p,
											 sf::TcpSocket*    _conn) {
	*_p[0] << m_server.m_world->getTime();
}

void NetworkManagerServer::receiveDataPlayer(sf::Packet*    _p,
                                             sf::TcpSocket* _conn) {
	ComponentsPlayer p;
	*_p >> p;
	m_server.updatePlayer(p);

	sendPacket(Packet::UDP::DATA_PLAYER, _conn);
}
