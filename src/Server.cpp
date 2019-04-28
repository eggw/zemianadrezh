#include "Server.h"

Server::Server(bool _allowConnections)
            : GameInstance(),
              m_networkManager(*this),
              m_allowConnections(_allowConnections) {
	m_players = std::make_shared<std::vector<std::shared_ptr<Player>>>();

	m_world.addChunk(0, 2, false);
}

Server::~Server() {
	m_networkManager.sendPacket(Packet::TCPPacket::CONNECTIONLOST);
}

void Server::updatePlayer(const Player::EncodedPlayerData& _data) {
	for (auto& player : *m_players) {
		if (player->getName() == _data.playerName) {
			player->parseData(_data);
		}
	}
}

void Server::addPlayer(const Player::EncodedPlayerData& _data) {
	auto newPlayer{std::make_shared<Player>(_data.playerName)};
	m_players->push_back(std::move(newPlayer));
}

void Server::receivePackets() {
	m_networkManager.receiveUDPPackets();
	m_networkManager.receiveTCPPackets();
}

void Server::update(int _timeslice) {
	//TODO: figure out how to keep server/client world times synced
	//m_world.update(_timeslice);

	if (m_players != nullptr) {
		for (auto& player : *m_players) {
			player->update(_timeslice);
		}
	}

	m_networkManager.accept();
	m_networkManager.update();
}

bool Server::connectionsAllowed() {
	return m_allowConnections;
}
