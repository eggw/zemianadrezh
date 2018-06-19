#include "GameInstance.h"

GameInstance::GameInstance()
    :m_world(),
     m_otherPlayers() {

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

void GameInstance::updateOtherPlayers(Player::EncodedPlayerData _data){
	bool found{false};

	for(auto& player : m_otherPlayers){
		if(player->getName() == _data.playerName){
			player->parseData(_data);
			found = true;
		}
	}

	if(!found){
        auto newPlayer = std::unique_ptr<Player>(new Player());
        newPlayer->parseData(_data);
        m_otherPlayers.push_back(std::move(newPlayer));
	}
}
