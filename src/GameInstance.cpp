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

const World::EncodedWorldData GameInstance::encodeWorldData() const {
	return m_world.encodeData();
}

void GameInstance::parseWorldData(const World::EncodedWorldData& _data) {
	m_world.parseData(_data);
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
