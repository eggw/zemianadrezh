#include "NetworkManagerClient.h"

#include "LoggerNetwork.h"

#include <iostream>

#include "Client.h"

NetworkManagerClient::NetworkManagerClient(Client& _client)
    :m_client(_client) {

    m_serverConnection.setBlocking(false);
}

void NetworkManagerClient::sendPacket(Packet::Type _type) {
    int packetCode = Packet::toInt(_type);
    sf::Packet packet;
    packet << packetCode;

    switch(_type) {

    case Packet::Type::REQUEST_WORLD:
        m_serverConnection.send(packet);
        std::cout << "CLIENT: sent world request" << std::endl;
        break;
    default:
        break;
    }
}


void NetworkManagerClient::receivePacket() {
    int packetCode;
    sf::Packet packet;

    if(m_serverConnection.receive(packet) == sf::Socket::Status::Done) {
        packet >> packetCode;
        Packet::Type packetType{Packet::toType(packetCode)};
        std::cout << "CLIENT: received: " << packetCode << std::endl;

        switch(packetType) {
        case Packet::Type::DATA_WORLD:{

            World::EncodedWorldData worldData;

            packet >> worldData.chunkIDs;
            packet >> worldData.invisibleBlocks;

            m_client.parseWorldData(worldData);
            break;
        }
        default:
            break;
        }
    }
}

void NetworkManagerClient::connect(sf::IpAddress _ip, int _port) {
    sf::Socket::Status status = m_serverConnection.connect(_ip, _port);

    if(status != sf::Socket::Done) {
        LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::CLIENT,
                                          LoggerNetwork::LOG_MESSAGE::CONNECTION_FAILURE);
    }
    else {
        LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::CLIENT,
                                          LoggerNetwork::LOG_MESSAGE::CONNECTION_SUCCESS);
    }
}