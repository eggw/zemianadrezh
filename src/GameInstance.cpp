#include "GameInstance.h"

GameInstance::GameInstance()
    :m_world(),
     m_otherPlayers(nullptr) {

}

const World& GameInstance::getWorld() const {
    return m_world;
}

const std::shared_ptr<std::vector<std::unique_ptr<Player>>> GameInstance::getOtherPlayers() const{
	return m_otherPlayers;
}

const World::EncodedWorldData GameInstance::encodeWorldData() const {
    return m_world.encodeData();
}

void GameInstance::parseWorldData(World::EncodedWorldData& _data) {
    m_world.parseData(_data);
}
