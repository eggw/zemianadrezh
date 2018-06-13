#include "Server.h"
#include <iostream>

Server::Server()
    :m_listener(),
     m_world() {

}


const World& Server::getWorld() const {
    return m_world;
}

void Server::listen() {
    if(m_listener.listen(7777) != sf::Socket::Done) {
        std::cout << "Error listening to port" << std::endl;
    }
}

void Server::accept() {
    m_clientConnections.push_back(std::unique_ptr<sf::TcpSocket>(new sf::TcpSocket()));

    sf::Socket::Status status = m_listener.accept(*m_clientConnections.back());
    if(status != sf::Socket::Done) {
        std::cout << "Error!" << std::endl;
        m_clientConnections.pop_back();
    }
    else {
        std::cout << "SERVER: Success!" << std::endl;
        sf::Packet worldPacket = m_world.sendData();
        m_clientConnections.back()->send(worldPacket);
    }
}
