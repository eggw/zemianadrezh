#include "PacketSender.h"


PacketSender::PacketSender() {

}

//Attempts to send the packet, and if not successful, moves
//it along with its sending socket into m_packetData, where
//sending the very same packet object can be attempted again
void PacketSender::send(sf::TcpSocket* _socket, PacketUPtr _packet) {
    PacketData data{_socket, std::move(_packet)};

    auto status = data.m_socket->send(*(data.m_packet));
    if(status == sf::Socket::Status::Partial) {
        m_packetData.push_back(std::move(data));
    }
}

//Loops through m_packetData, attempts to send the packets,
//and removes them from the vector if the attempt was successful
void PacketSender::update() {
    m_packetData.erase(std::remove_if(
                           m_packetData.begin(),
                           m_packetData.end(),
    [](const PacketData& p) {
        //return true to have it removed
        auto status = p.m_socket->send(*(p.m_packet));
        return (status == sf::Socket::Status::Done);
    }), m_packetData.end());
}
