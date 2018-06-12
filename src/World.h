#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include "WorldChunk.h"

#include "Singleton.h"

class World: public Singleton<World> {
  public:
    World();
	const std::vector<WorldChunk>& getChunks() const;
  private:
    std::vector<WorldChunk> m_chunks;
};


#endif // WORLD_H_INCLUDED
