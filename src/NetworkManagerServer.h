#ifndef NETWORKMANAGERSERVER_H_INCLUDED
#define NETWORKMANAGERSERVER_H_INCLUDED

#include <memory>

#include "SFML/Network.hpp"

#include "PacketTypes.h"

class Server;

class NetworkManagerServer {
    public:
        NetworkManagerServer(Server& _server);

        //The second and third arguments here are optional.
        //Depending on whether _exclude is true or false,
        //the packet will either be sent to everyone but the
        //specificed recipient (true), or only include the specified
        //recipient (false, default).
        void sendPacket(Packet::Type _type,
                        sf::TcpSocket* _recipient = nullptr,
                        bool _exclude = false);

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
