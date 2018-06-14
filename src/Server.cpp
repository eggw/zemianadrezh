#include "Server.h"

Server::Server()
    :GameInstance(),
     m_networkManager(*this) {

}

void Server::update(){
    m_networkManager.receivePacket();
}
