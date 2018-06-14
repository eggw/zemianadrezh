#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "NetworkManagerClient.h"

#include "GameInstance.h"

/*
The client class takes an optional pointer to a server.
This pointer should only be passed if the server is local;
if it's passed, the client will use the same resources as the
server.
*/

class Server;

class Client : public GameInstance {
    public:
        Client(Server* _localServer = nullptr);

        void update();

        bool isLocal() const;
    public:
        NetworkManagerClient m_networkManager;
    private:
        Server* m_localServer;
};

#endif // CLIENT_H_INCLUDED
