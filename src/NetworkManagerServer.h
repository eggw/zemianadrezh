#ifndef NETWORKMANAGERSERVER_H_INCLUDED
#define NETWORKMANAGERSERVER_H_INCLUDED

#include <memory>

#include "SFML/Network.hpp"

#include "PacketTypes.h"

class Server;

class NetworkManagerServer {
    public:
        NetworkManagerServer(Server& _server);

        void sendPacket(Packet::Type _type, sf::TcpSocket* _recipient = nullptr);
        void receivePacket();

        const std::vector<std::unique_ptr<sf::TcpSocket>>& getClients();

        void listen();
        void accept();
    private:
        Server& m_server;
        sf::TcpListener m_listener;
        std::vector<std::unique_ptr<sf::TcpSocket>> m_clientConnections;
};


#endif // NETWORKMANAGERSERVER_H_INCLUDED
