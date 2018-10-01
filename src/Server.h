#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <memory>

#include "NetworkManagerServer.h"

#include "GameInstance.h"

class Server : public GameInstance {
    public:
        //todo: pass world settings to constructor
        Server(bool _allowConnections);

        void updateOtherPlayers(Player::EncodedPlayerData _data);
        void receivePackets();

        void update();

        bool connectionsAllowed();
    public:
        NetworkManagerServer m_networkManager;
        bool m_allowConnections;
};

#endif // SERVER_H_INCLUDED
