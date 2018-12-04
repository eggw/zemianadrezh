#include "GameInstance.h"

GameInstance::GameInstance()
	:m_world(),
	 m_players(nullptr) {

}

const World& GameInstance::getWorld() const {
	return m_world;
}

const std::shared_ptr<std::vector<std::unique_ptr<Player>>> GameInstance::getPlayers() const {
	return m_players;
}

const World::EncodedWorldData GameInstance::encodeWorldData() const {
	return m_world.encodeData();
}

void GameInstance::parseWorldData(World::EncodedWorldData& _data) {
	m_world.parseData(_data);
}