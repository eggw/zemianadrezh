#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <memory>

#include "NetworkManagerServer.h"

#include "World.h"

class Server {
  public:
    Server();

    const World& getWorld() const;

  public:
	NetworkManagerServer m_networkManager;

  private:
    World m_world;
};

#endif // SERVER_H_INCLUDED
