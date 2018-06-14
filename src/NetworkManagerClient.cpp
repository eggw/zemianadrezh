#include "NetworkManagerClient.h"

#include "LoggerNetwork.h"

#include <iostream>

NetworkManagerClient::NetworkManagerClient(Client& _client)
    :m_client(_client) {

}

void NetworkManagerClient::sendPacket(Packet::Type _type) {

}


void NetworkManagerClient::receivePacket() {
    int packetCode;
    sf::Packet packet;

    if(m_serverConnection.receive(packet) == sf::Socket::Status::Done) {
        packet >> packetCode;
        Packet::Type packetType{Packet::toType(packetCode)};
        std::string str;
        packet >> str;
        std::cout << "received: " << packetCode << ", " << str << std::endl;
    }
}

void NetworkManagerClient::connect(sf::IpAddress _ip, int _port) {
    sf::Socket::Status status = m_serverConnection.connect(_ip, _port);

    if(status != sf::Socket::Done) {
        LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::CLIENT,
                                          LoggerNetwork::LOG_MESSAGE::CONNECTION_FAILURE);
    } else {
        LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::CLIENT,
                                          LoggerNetwork::LOG_MESSAGE::CONNECTION_SUCCESS);
    }
}
