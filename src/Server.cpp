#include "Server.h"

#include "LoggerNetwork.h"

Server::Server()
    :m_networkManager(),
     m_world() {

}

const World& Server::getWorld() const {
    return m_world;
}
