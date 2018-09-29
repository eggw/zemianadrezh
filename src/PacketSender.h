#ifndef PACKETSENDER_H_INCLUDED
#define PACKETSENDER_H_INCLUDED

#include <SFML/Network.hpp>

#include "Singleton.h"


class PacketSender : public Singleton<PacketSender> {
    public:
        struct PacketData {
            PacketData(sf::TcpSocket* _socket, sf::Packet* _packet)
            :   m_socket{_socket},
                m_packet{_packet}{}

            sf::TcpSocket* m_socket;
            sf::Packet* m_packet;
        };

        PacketSender();

        void send(sf::TcpSocket* _socket, sf::Packet* _packet);
        void update();

    private:
        std::vector<PacketData> m_packetData;

};


#endif // PACKETSENDER_H_INCLUDED
