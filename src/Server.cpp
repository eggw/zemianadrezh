#include "Server.h"

Server::Server()
    :GameInstance(),
     m_networkManager(*this) {

    m_world.addChunk(5);

}

void Server::updateOtherPlayers(Player::EncodedPlayerData _data) {
    if(m_otherPlayers == nullptr) {
        m_otherPlayers = std::shared_ptr<std::vector<std::unique_ptr<Player>>>
                         (new std::vector<std::unique_ptr<Player>>());
    }

    bool found{false};

    for(auto& player : *m_otherPlayers) {
        if(player->getName() == _data.playerName) {
            player->parseData(_data);
            found = true;
        }
    }

    if(!found) {
        auto newPlayer = std::unique_ptr<Player>(new Player());
        newPlayer->parseData(_data);
        m_otherPlayers->push_back(std::move(newPlayer));
    }
}

void Server::receivePackets() {
    m_networkManager.receivePacket();
}
