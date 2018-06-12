#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include "SFML/Network.hpp"

#include "World.h"
class Server {
  public:
    Server();

    const World& getWorld() const;

    void listen();
    void accept();
  private:
    sf::TcpListener m_listener;
    sf::TcpSocket m_clientConnection;

    World m_world;
};

#endif // SERVER_H_INCLUDED
