#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <memory>

#include "NetworkManagerServer.h"

#include "GameInstance.h"

class Server : public GameInstance {
    public:
        Server();

        void update();
    public:
        NetworkManagerServer m_networkManager;
};

#endif // SERVER_H_INCLUDED
