#include "Server.h"

Server::Server(bool _allowConnections)
	:GameInstance(),
	 m_networkManager(*this),
	 m_allowConnections(_allowConnections) {

	m_otherPlayers = std::shared_ptr<std::vector<std::unique_ptr<Player>>>
					 (new std::vector<std::unique_ptr<Player>>());

	m_world.addChunk(1);
}

void Server::updateOtherPlayers(Player::EncodedPlayerData _data) {
	for(auto& player : *m_otherPlayers) {
		if(player->getName() == _data.playerName) {
			player->parseData(_data);
		}
	}
}

void Server::addPlayer(Player::EncodedPlayerData _data) {
	auto newPlayer = std::unique_ptr<Player>(new Player(_data.playerName));
	respawnPlayer(_data.playerName);
	m_otherPlayers->push_back(std::move(newPlayer));
}

void Server::respawnPlayer(std::string& _name) {
	for(auto& player : *m_otherPlayers) {
		if(player->getName() == _name) {
			player->setPosition(m_world.getCenter());
		}
	}
}

void Server::receivePackets() {
	m_networkManager.receivePacket();
}

void Server::update() {
	if(m_otherPlayers != nullptr) {
		for(auto& player : *m_otherPlayers) {
			player->update();
		}
	}

	m_networkManager.accept();
	m_networkManager.update();
}

bool Server::connectionsAllowed() {
	return m_allowConnections;
}
