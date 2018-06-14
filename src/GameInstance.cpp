#include "GameInstance.h"

GameInstance::GameInstance()
    :m_world() {

}

const World& GameInstance::getWorld() const {
    return m_world;
}

const World::EncodedWorldData GameInstance::encodeWorldData() const {
	return m_world.encodeData();
}

void GameInstance::parseWorldData(World::EncodedWorldData& _data) {
    m_world.parseData(_data);
}

