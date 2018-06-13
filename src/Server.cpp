#include "Server.h"

#include "LoggerNetwork.h"

Server::Server()
    :m_listener(),
     m_world() {

}

const World& Server::getWorld() const {
    return m_world;
}

void Server::listen() {
    if(m_listener.listen(7777) != sf::Socket::Done) {
        LoggerNetwork::get_instance().log(	LoggerNetwork::LOG_SENDER::SERVER,
											LoggerNetwork::LOG_MESSAGE::LISTEN_PORT_FAILURE);
    }
    else{
        LoggerNetwork::get_instance().log(	LoggerNetwork::LOG_SENDER::SERVER,
											LoggerNetwork::LOG_MESSAGE::LISTEN_PORT_SUCCESS);
    }
}

void Server::accept() {
    m_clientConnections.push_back(std::unique_ptr<sf::TcpSocket>(new sf::TcpSocket()));

    sf::Socket::Status status = m_listener.accept(*m_clientConnections.back());
    if(status != sf::Socket::Done) {
        LoggerNetwork::get_instance().log(	LoggerNetwork::LOG_SENDER::SERVER,
											LoggerNetwork::LOG_MESSAGE::CONNECTION_FAILURE);
        m_clientConnections.pop_back();
    }
    else {
        LoggerNetwork::get_instance().log(	LoggerNetwork::LOG_SENDER::SERVER,
											LoggerNetwork::LOG_MESSAGE::CONNECTION_SUCCESS);
        //send world data
    }
}
