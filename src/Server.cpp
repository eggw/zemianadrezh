#include "Server.h"

Server::Server()
    :GameInstance(),
     m_networkManager(*this) {

    m_world.addChunk(2);

}

void Server::update() {
    m_networkManager.receivePacket();
}
