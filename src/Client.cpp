#include "Client.h"

#include "Server.h"

#include "LoggerNetwork.h"
#include "TextureManager.h"

#include "Tags/PlayerTag.h"
#include "Components/ComponentAnimation.h"
#include "Components/ComponentsPlayer.h"

Client::Client(sf::RenderWindow& _window,
               sf::IpAddress     _serverIP,
               Server*           _localServer)
            : GameInstance(),
              m_networkManager(*this),
              m_serverIP(_serverIP),
              m_localServer(_localServer),
              m_player{m_registry.create()},
              //TODO:
              //ChatBox's second parameter should take in the player's name more...
              //elegantly
              m_interface(_window,
						  m_networkManager,
						  _localServer == nullptr ? "RemotePlayer" : "LocalPlayer") {

	initialisePlayer();
	/*
	TODO/TOFIX
	Assigning m_world and m_players to the server's references doesn't properly work.
	To test it, comment out m_world.update() in Client.cpp and uncomment it in Server.cpp

	Even when they're supposed to be shared (i.e. Client's m_world is supposed to refer to
	Server's m_world), that doesn't seem to be the case. Probably because m_world is init-
	ialised in GameInstance first, but I need to investigate this further.
	*/
	if (m_localServer != nullptr) {
		m_world = m_localServer->getWorld();

		LoggerNetwork::get_instance().log(
		  LoggerNetwork::LOG_SENDER::CLIENT,
		  LoggerNetwork::LOG_MESSAGE::CONNECTION_LOCALHOST);
	}
}

Client::~Client() {
	if (!isLocal()) {
		m_networkManager.sendPacket(Packet::TCPPacket::QUIT);
	}
}

void Client::getInput(sf::Event& _event) {
	m_interface.getInput(_event);
}

void Client::update(int _timeslice) {
	m_world.update(_timeslice);
	m_networkManager.update();
	m_interface.update(_timeslice);
}

void Client::drawInterface() const {
	m_interface.draw();
}

void Client::sendPackets() {
	//Ensure that a reasonable rate of packet sending is maintained.
	static sf::Clock c;
	static float     secondsBetweenPackets{0.015};

	if (c.getElapsedTime().asSeconds() >= secondsBetweenPackets) {
		m_networkManager.sendPacket(Packet::UDPPacket::DATA_PLAYER);
		c.restart();
	}
}

void Client::receivePackets() {
	m_networkManager.receiveUDPPackets();
	m_networkManager.receiveTCPPackets();
}

entt::entity Client::getPlayerId() const {
	return m_player;
}

sf::Vector2f Client::getPlayerPosition() const {
	return m_registry.get<ComponentPosition>(m_player).m_position;
}

bool Client::isConnected() const {
	return m_networkManager.connectionActive();
}

bool Client::isLocal() const {
	return m_localServer != nullptr;
}

void Client::initialisePlayer() {
	ComponentsPlayer p{};
	p.compName.m_name = (m_localServer == nullptr ? "RemotePlayer" : "LocalPlayer");
	addPlayer(p, m_player);
}

void Client::respawnPlayer() {
	auto& playerPos = m_registry.get<ComponentPosition>(m_player).m_position;
	playerPos.x = m_world.getCenter().x;
	playerPos.y = 0;
}
