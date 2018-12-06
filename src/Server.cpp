#include "Server.h"

Server::Server(bool _allowConnections)
	:GameInstance(),
	 m_networkManager(*this),
	 m_allowConnections(_allowConnections) {

	m_players = std::shared_ptr<std::vector<std::shared_ptr<Player>>>
					 (new std::vector<std::shared_ptr<Player>>());

	m_world.addChunk(2, true);
}

void Server::updatePlayer(Player::EncodedPlayerData _data) {
	for(auto& player : *m_players) {
		if(player->getName() == _data.playerName) {
			player->parseData(_data);
		}
	}
}

void Server::addPlayer(Player::EncodedPlayerData _data) {
	auto newPlayer = std::unique_ptr<Player>(new Player(_data.playerName));
	m_players->push_back(std::move(newPlayer));
}

void Server::receivePackets() {
	m_networkManager.receiveUDPPackets();
	m_networkManager.receiveTCPPackets();
}

void Server::update() {
	if(m_players != nullptr) {
		for(auto& player : *m_players) {
			player->update();
		}
	}

	m_networkManager.accept();
	m_networkManager.update();
}

bool Server::connectionsAllowed() {
	return m_allowConnections;
}
