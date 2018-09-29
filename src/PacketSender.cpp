#include "PacketSender.h"


PacketSender::PacketSender() {

}

void PacketSender::send(sf::TcpSocket* _socket, sf::Packet* _packet) {
    PacketData data{_socket, _packet};

    auto status = data.m_socket->send(*_packet);
    if(status == sf::Socket::Status::Partial){
        m_packetData.push_back(data);
    }
}

void PacketSender::update() {
    for(auto& packetData : m_packetData){
        auto status = packetData.m_socket->send(*(packetData.m_packet));
        if(status == sf::Socket::Status::Done){
            //TODO::
            //Remove this element from the vector
        }
    }
}
