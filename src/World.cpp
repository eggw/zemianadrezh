#include "World.h"

World::World() {
	m_chunks.push_back(0);
	m_chunks.push_back(1);
}

const std::vector<WorldChunk>& World::getChunks() const {
	return m_chunks;
}

sf::Packet World::sendData() {
    sf::Packet worldPacket;
    return worldPacket;
}

void World::parseData(sf::Packet _packet) {
}
