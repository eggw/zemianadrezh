#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <memory>

#include "NetworkManagerServer.h"

#include "GameInstance.h"

class Server : public GameInstance {
    public:
        Server();

        void updateOtherPlayers(Player::EncodedPlayerData _data);
        void receivePackets();
    public:
        NetworkManagerServer m_networkManager;
};

#endif // SERVER_H_INCLUDED
