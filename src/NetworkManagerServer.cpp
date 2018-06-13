#include "NetworkManagerServer.h"

#include "LoggerNetwork.h"

#include <iostream>

NetworkManagerServer::NetworkManagerServer()
    : m_listener() {

    m_listener.setBlocking(false);
    listen();
}

void NetworkManagerServer::sendPacket(Packet::Type _type) {
    int packetCode = Packet::toInt(_type);
    sf::Packet packet;
    packet << packetCode;

    switch(_type) {

    case Packet::Type::DATA_WORLD:
        for(auto& client : m_clientConnections) {
            packet << "world info here";

            client->send(packet);
            std::cout << "Sent world packet" << std::endl;
        }

        break;
    }
}

void NetworkManagerServer::receivePacket() {

}


const std::vector<std::unique_ptr<sf::TcpSocket>>& NetworkManagerServer::getClients() {
    return m_clientConnections;
}

void NetworkManagerServer::listen() {
    if(m_listener.listen(7777) != sf::Socket::Done) {
        LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::SERVER,
                                          LoggerNetwork::LOG_MESSAGE::LISTEN_PORT_FAILURE);
    } else {
        LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::SERVER,
                                          LoggerNetwork::LOG_MESSAGE::LISTEN_PORT_SUCCESS);
    }
}

void NetworkManagerServer::accept() {
    m_clientConnections.push_back(std::unique_ptr<sf::TcpSocket>(new sf::TcpSocket()));
    m_clientConnections.back()->setBlocking(false);

    sf::Socket::Status status = m_listener.accept(*m_clientConnections.back());
    if(status != sf::Socket::Done) {
        LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::SERVER,
                                          LoggerNetwork::LOG_MESSAGE::CONNECTION_FAILURE);
        m_clientConnections.pop_back();
    } else {
        LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::SERVER,
                                          LoggerNetwork::LOG_MESSAGE::CONNECTION_SUCCESS);
    }
}
