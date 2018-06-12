#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include "SFML/Network.hpp"

#include "World.h"

#include "Singleton.h"

class Server : public Singleton<Server> {
  public:
    Server();

    const World& getWorld() const;
  private:
  	sf::TcpSocket m_socket;

    World m_world;
};

#endif // SERVER_H_INCLUDED
