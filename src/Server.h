#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <memory>

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
    std::vector<std::unique_ptr<sf::TcpSocket>> m_clientConnections;

    World m_world;
};

#endif // SERVER_H_INCLUDED
