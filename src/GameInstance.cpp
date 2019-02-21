#include "GameInstance.h"

GameInstance::GameInstance()
	:m_world(),
	 m_players(nullptr) {

}

const World& GameInstance::getWorld() const {
	return m_world;
}

const std::shared_ptr<std::vector<std::shared_ptr<Player>>> GameInstance::getPlayers() const {
	return m_players;
}

const encodedChunks GameInstance::encodeWorldChunks() const {
	return m_world.encodeChunks();
}

void GameInstance::parseWorldChunk(const WorldChunk::EncodedChunkData& _data) {
	m_world.parseChunk(_data);
}

void GameInstance::removePlayer(std::string _name) {
	m_players->erase(std::remove_if(
						   m_players->begin(),
						   m_players->end(),
	[&](const auto& p) {
		//return true to have it removed
		return (p.get()->getName() == _name);
	}), m_players->end());
}
