#include "NetworkManagerClient.h"

#include <iostream>
#include <sstream>

#include "LoggerNetwork.h"
#include "Client.h"

#include "Tags/PlayerTag.h"
#include "Components/ComponentsPlayer.h"

NetworkManagerClient::NetworkManagerClient(Client& _client)
            : m_client{_client},
              m_playerSpawned{false},
              m_connectionActive{false},
              m_udpSocket{},
			  m_TCPSender{},
			  m_TCPReceiver{},
			  m_UDPSender{},
			  m_UDPReceiver{} {
	m_udpSocket.setBlocking(false);

	if (m_udpSocket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
		LoggerNetwork::get_instance().log(
		  LoggerNetwork::LOG_SENDER::CLIENT,
		  LoggerNetwork::LOG_MESSAGE::BIND_PORT_FAILURE);
	}
	else {
		LoggerNetwork::get_instance().log(
		  LoggerNetwork::LOG_SENDER::CLIENT,
		  LoggerNetwork::LOG_MESSAGE::BIND_PORT_SUCCESS);
	}

	m_serverConnection.setBlocking(false);

	//Here, we use m_processor to specify which functions we want to be called
	//when which packets are sent or received.
	//------------------------------------------------------------------------------
	using std::placeholders::_1;

	m_TCPSender.add(Packet::TCP::JUST_JOINED, std::bind(&NetworkManagerClient::sendJustJoined, this, _1));
	m_TCPSender.add(Packet::TCP::CHAT_MESSAGE, std::bind(&NetworkManagerClient::sendChatMessage, this, _1));

	m_UDPSender.add(Packet::UDP::DATA_PLAYER, std::bind(&NetworkManagerClient::sendDataPlayer, this, _1));

	m_TCPReceiver.add(Packet::TCP::QUIT, std::bind(&NetworkManagerClient::receiveQuit, this, _1));
	m_TCPReceiver.add(Packet::TCP::CONNECTIONLOST, std::bind(&NetworkManagerClient::receiveConnectionLost, this, _1));
	m_TCPReceiver.add(Packet::TCP::CHAT_MESSAGE, std::bind(&NetworkManagerClient::receiveChatMessage, this, _1));
	m_TCPReceiver.add(Packet::TCP::DATA_WORLD, std::bind(&NetworkManagerClient::receiveDataWorld, this, _1));
	m_TCPReceiver.add(Packet::TCP::RESPAWN_PLAYER, std::bind(&NetworkManagerClient::receiveRespawnPlayer, this, _1));

	m_UDPReceiver.add(Packet::UDP::DATA_PLAYER, std::bind(&NetworkManagerClient::receiveDataPlayer, this, _1));
	//------------------------------------------------------------------------------
}

void NetworkManagerClient::update() {
	PacketSender::get_instance().update();
}

void NetworkManagerClient::sendPacket(Packet::TCP _type) {
	int             packetCode = Packet::toInt(_type);
	PacketSharedPtr packet(new sf::Packet());
	*packet << packetCode;

	m_TCPSender.call(_type, packet.get());

	PacketSender::get_instance().send(&m_serverConnection, packet);
}

void NetworkManagerClient::sendPacket(Packet::UDP _type) {
	//m_playerSpawned is set to true once a RESPAWN::PLAYER packet
	//has been received at least once. Until then, we're not going
	//to want to send any UDP packets.
	if (!m_playerSpawned) {
		return;
	}

	int             packetCode = Packet::toInt(_type);
	PacketSharedPtr packet(new sf::Packet());
	*packet << packetCode;
	unsigned short port{Packet::Port_UDP_Server};

	m_UDPSender.call(_type, packet.get());

	PacketSender::get_instance().send(
		  &m_udpSocket, packet, m_serverConnection.getRemoteAddress(), port);
}

void NetworkManagerClient::receiveTCPPackets() {
	int        packetCode;
	PacketUPtr packet(new sf::Packet());

	while (m_serverConnection.receive(*packet) == sf::Socket::Status::Done) {
		*packet >> packetCode;
		Packet::TCP packetType{Packet::toTCPType(packetCode)};

		m_TCPReceiver.call(packetType, packet.get());
	}
}

//This function is used to display the number of received packets per second as long
//as they're not 0. Used in the beginning of receiveUDPPackets()
void logToConsole(int& _i) {
	static sf::Clock clock{};
	if (clock.getElapsedTime().asSeconds() >= 1) {
		if (_i > 0) {
			std::cout << "[CLIENT] UDP packets received: " << _i << std::endl;
			std::cout << "----------------------------------" << std::endl;
			_i = 0;
		}
		clock.restart();
	}
}

void NetworkManagerClient::receiveUDPPackets() {
	static int UDPPacketsReceived{0};
	logToConsole(UDPPacketsReceived);

	int            packetCode;
	PacketUPtr     packet(new sf::Packet());
	unsigned short port{m_udpSocket.getLocalPort()};

	sf::IpAddress address{m_serverConnection.getRemoteAddress()};

	while (m_udpSocket.receive(*packet, address, port) ==
	       sf::Socket::Status::Done) {
		UDPPacketsReceived++;
		*packet >> packetCode;
		Packet::UDP packetType{Packet::toUDPType(packetCode)};

		m_UDPReceiver.call(packetType, packet.get());
	}
}

void NetworkManagerClient::connect(const sf::IpAddress& _ip,
                                   unsigned short       _port) {
	bool successfullyConnected{false};

	m_serverConnection.setBlocking(true);
	sf::Socket::Status status = m_serverConnection.connect(_ip, _port);

	if (status != sf::Socket::Done) {
		LoggerNetwork::get_instance().log(
		  LoggerNetwork::LOG_SENDER::CLIENT,
		  LoggerNetwork::LOG_MESSAGE::CONNECTION_WAITING);
	}
	else {
		LoggerNetwork::get_instance().log(
		  LoggerNetwork::LOG_SENDER::CLIENT,
		  LoggerNetwork::LOG_MESSAGE::CONNECTION_SUCCESS);
		successfullyConnected = true;
	}

	m_serverConnection.setBlocking(false);

	if (successfullyConnected) {
		m_connectionActive = true;
		sendPacket(Packet::TCP::JUST_JOINED);
	}
}

bool NetworkManagerClient::connectionActive() const {
	bool invalidHost{m_serverConnection.getRemoteAddress() ==
	                 sf::IpAddress::None};
	return m_connectionActive && !invalidHost;
}

bool NetworkManagerClient::receivedMessage(Message* _ptr) {
	bool valid{m_lastReceivedMessage.sender != ""};

	if (_ptr != nullptr) {
		*_ptr = m_lastReceivedMessage;
	}

	return valid;
}

void NetworkManagerClient::clearLastReceivedMessage() {
	m_lastReceivedMessage.sender  = "";
	m_lastReceivedMessage.content = "";
}

void NetworkManagerClient::setMessageToSend(Message _msg) {
	m_messageToSend = _msg;
}

void NetworkManagerClient::sendJustJoined(sf::Packet* _p) {
	entt::entity e{m_client.m_player};
	sf::Uint16   port{m_udpSocket.getLocalPort()};

	const auto dir{m_client.m_registry->get<ComponentDirection>(e)};
	const auto name{m_client.m_registry->get<ComponentName>(e)};
	const auto vel{m_client.m_registry->get<ComponentPhysics>(e)};
	const auto pos{m_client.m_registry->get<ComponentPosition>(e)};

	ComponentsPlayer data{dir, name, vel, pos};

	*_p << data;
	*_p << port;
}


void NetworkManagerClient::sendChatMessage(sf::Packet* _p) {
	//At this stage, m_messageToSend should have been set (usually by
	//UserInterface)
	*_p << m_messageToSend.sender;
	*_p << m_messageToSend.content;
}


void NetworkManagerClient::receiveQuit(sf::Packet* _p) {
	std::string name{};
	*_p >> name;
	m_client.removePlayer(name);
}
void NetworkManagerClient::receiveConnectionLost(sf::Packet* _p) {
	m_connectionActive = false;
}

void NetworkManagerClient::receiveChatMessage(sf::Packet* _p) {
	std::string sender;
	std::string message;
	*_p >> sender;
	*_p >> message;

	m_lastReceivedMessage = {sender, message};
}

void NetworkManagerClient::receiveDataWorld(sf::Packet* _p) {
	WorldChunk::EncodedChunkData chunkData;
	*_p >> chunkData;

	//We won't want to parse the world data
	//if we're local, since the world will
	//be a shared pointer anyway, and re-parsing
	//will involve calling addChunks and mess up
	//the world by un-randomizing it
	if (!m_client.isLocal()) {
		m_client.parseWorldChunk(chunkData);
	}

	m_client.renderUpdatedChunk(chunkData.id);
}

void NetworkManagerClient::receiveRespawnPlayer(sf::Packet* _p) {
	m_client.respawnPlayer();
	m_playerSpawned = true;
}

///////////////////////////////////////
void NetworkManagerClient::sendDataPlayer(sf::Packet* _p) {
	entt::entity e{m_client.m_player};

	auto dir{m_client.m_registry->get<ComponentDirection>(e)};
	auto name{m_client.m_registry->get<ComponentName>(e)};
	auto vel{m_client.m_registry->get<ComponentPhysics>(e)};
	auto pos{m_client.m_registry->get<ComponentPosition>(e)};

	*_p << ComponentsPlayer{dir, name, vel, pos};
}

void NetworkManagerClient::receiveDataPlayer(sf::Packet* _p) {
	ComponentsPlayer p;
	*_p >> p;
	m_client.updatePlayer(p);
}
