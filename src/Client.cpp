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
              //get back to this:
              //ChatBox's second parameter should take in the player's name
              m_chatBox(_window, "TEST"),
              m_userInterface(_window) {
	m_registry.assign<PlayerTag>(m_player, true);
	m_registry.assign<ComponentAnimation>(
	  m_player,
	  TextureManager::get_instance().getTexture(TextureManager::Type::PLAYER));

	m_registry.assign<ComponentName>(
	  m_player, _localServer == nullptr ? "RemotePlayer" : "LocalPlayer");

	m_registry.assign<ComponentPhysics>(m_player);
	m_registry.assign<ComponentDirection>(m_player, false);
	m_registry.assign<ComponentPosition>(m_player, 0, 0);
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
	m_chatBox.getInput(_event);
	m_userInterface.getInput(_event);
}

void Client::update(int _timeslice) {
	m_world.update(_timeslice);

	handleIncomingMessages();
	handleOutgoingMessages();
	m_networkManager.update();

	m_chatBox.update();
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

//Searches the registry for an entity with a PlayerTag specifiying
//a local player, and returns their position
sf::Vector2f Client::getPlayerPosition() const {
	auto view = m_registry.view<const PlayerTag>();
	for (auto entity : view) {
	    if (view.get(entity).m_local) {
	        return m_registry.get<ComponentPosition>(entity).m_position;
	    }
	}
	return {0, 0};
}

const ChatBox* Client::getChatBox() const {
	return &m_chatBox;
}

const UserInterface* Client::getUserInterface() const {
	return &m_userInterface;
}

bool Client::isConnected() const {
	return m_networkManager.connectionActive();
}

bool Client::isLocal() const {
	return m_localServer != nullptr;
}

std::pair<std::string, std::string> Client::getPendingMessage() const {
	return m_pendingMessage;
}

//We check to see if any new messages have been received from our
//network manager. If so, we'll add it to the chatbox and clear it from
//the network manager.
void Client::handleIncomingMessages() {
	std::unique_ptr<std::pair<std::string, std::string>> ptr(
	  new std::pair<std::string, std::string>);
	if (m_networkManager.receivedMessage(ptr.get())) {
		m_chatBox.appendMessage(ptr->first, ptr->second);
		m_networkManager.clearLastReceivedMessage();
	}
}

//We check to see if there is any new outgoing message from the chatbox,
//and if so, we'll set it as our latest m_pendingMessage, then send it
//to the server
void Client::handleOutgoingMessages() {
	std::unique_ptr<std::pair<std::string, std::string>> ptr(
	  new std::pair<std::string, std::string>);
	if (m_chatBox.completedMessage(ptr.get())) {
		m_pendingMessage = *ptr;
		m_networkManager.sendPacket(Packet::TCPPacket::CHAT_MESSAGE);
	}
}

void Client::respawnPlayer() {
	auto& playerPos = m_registry.get<ComponentPosition>(m_player).m_position;
	playerPos.x = m_world.getCenter().x;
	playerPos.y = 0;
}
