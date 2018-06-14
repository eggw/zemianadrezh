#include "NetworkManagerServer.h"

#include <iostream>

#include "LoggerNetwork.h"

#include "Server.h"

NetworkManagerServer::NetworkManagerServer(Server& _server)
    : m_server(_server),
      m_listener() {

    m_listener.setBlocking(false);
    listen();
}

void NetworkManagerServer::sendPacket(Packet::Type _type) {
    int packetCode = Packet::toInt(_type);

    switch(_type) {

    case Packet::Type::DATA_WORLD:

        World::EncodedWorldData worldData = m_server.getWorld().encodeData();

        for(auto& client : m_clientConnections) {
            sf::Packet packet;
            packet << packetCode;
            packet << worldData.chunkIDs;
            packet << worldData.invisibleBlocks;
            client->send(packet);
            std::cout << "SERVER: Sent world packet" << std::endl;
        }

        break;
    }
}

void NetworkManagerServer::receivePacket() {
    int packetCode;
    sf::Packet packet;

    for(auto& connection : m_clientConnections){
            if(connection->receive(packet) == sf::Socket::Status::Done) {
            packet >> packetCode;
            Packet::Type packetType{Packet::toType(packetCode)};
            std::cout << "SERVER: received: " << packetCode << std::endl;

            switch(packetType){
            case Packet::Type::REQUEST_WORLD:
                sendPacket(Packet::Type::DATA_WORLD);
                break;
            default:
                break;
            }
        }
    }
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
