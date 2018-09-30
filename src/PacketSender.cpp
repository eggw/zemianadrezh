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

//Loops through m_packetData, attempts to send the packets,
//and removes them from the vector if the attempt was successful

void PacketSender::update() {
    m_packetData.erase(std::remove_if(
                m_packetData.begin(),
                m_packetData.end(),
                [](const PacketData& p){
                    //return true to have it removed
                    auto status = p.m_socket->send(*(p.m_packet));
                    return (status == sf::Socket::Status::Done);
                }), m_packetData.end());
}
