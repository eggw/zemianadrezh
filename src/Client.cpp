#include "Client.h"

#include "Server.h"

#include "LoggerNetwork.h"

#include <iostream>
Client::Client(sf::RenderWindow& _window, Server* _localServer)
	:GameInstance(),
	 m_networkManager(*this),
	 m_localServer(_localServer),
	 m_player(),
	 m_chatBox(_window, m_player.getName()) {

	if(m_localServer != nullptr) {
		m_world = m_localServer->getWorld();
		m_otherPlayers = m_localServer->getOtherPlayers();

		LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::CLIENT,
										  LoggerNetwork::LOG_MESSAGE::CONNECTION_LOCALHOST);
	}

}

void Client::getInput(sf::Event& _event) {
	m_player.getInput();
	m_chatBox.getInput(_event);
}

void Client::update() {
	m_player.update();
	m_chatBox.update();

	if(!isLocal()) {
		GameInstance::update();
	}
}

void Client::sendPackets() {
	m_networkManager.sendPacket(Packet::Type::DATA_PLAYER);
}

void Client::receivePackets() {
	m_networkManager.receivePacket();
}

void Client::respawnPlayer() {
	m_player.setPosition({m_world.getCenter().x, 0});
}

void Client::updateOtherPlayers(Player::EncodedPlayerData _data) {
	if(m_otherPlayers == nullptr) {
		m_otherPlayers = std::shared_ptr<std::vector<std::unique_ptr<Player>>>
						 (new std::vector<std::unique_ptr<Player>>());
	}

	if(_data.playerName == m_player.getName()) {
		return;
	}

	bool found{false};

	for(auto& player : *m_otherPlayers) {
		if(player->getName() == _data.playerName) {
			player->parseData(_data);
			found = true;
		}
	}

	if(!found) {
		auto newPlayer = std::unique_ptr<Player>(new Player());
		newPlayer->parseData(_data);
		m_otherPlayers->push_back(std::move(newPlayer));
	}
}

const Player* Client::getPlayer() const {
	return &m_player;
}

const ChatBox* Client::getChatBox() const{
    return &m_chatBox;
}

bool Client::isLocal() const {
	return m_localServer != nullptr;
}
