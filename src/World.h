#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include <SFML/Network.hpp>

#include "WorldChunk.h"

class World {
  public:
    World();
	const std::vector<WorldChunk>& getChunks() const;

    sf::Packet sendData();
    void parseData(sf::Packet _packet);
  private:
    std::vector<WorldChunk> m_chunks;
};


#endif // WORLD_H_INCLUDED
