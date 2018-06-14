#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include <SFML/Network.hpp>

#include "WorldChunk.h"

class World {
  public:
  	struct EncodedWorldData{
		std::string chunkIDs;
		std::string invisibleBlocks;
  	};

    World();

    void addChunk(int _num = 1);

	const std::vector<WorldChunk>& getChunks() const;
    const EncodedWorldData encodeData() const;
    void parseData(EncodedWorldData& _data);
  private:
    std::vector<WorldChunk> m_chunks;
};


#endif // WORLD_H_INCLUDED
