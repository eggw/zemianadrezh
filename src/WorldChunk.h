#ifndef WORLDCHUNK_H_INCLUDED
#define WORLDCHUNK_H_INCLUDED

#include "Block.h"

#include <vector>

constexpr int CHUNK_DIMENSIONS_X = 32;
constexpr int CHUNK_DIMENSIONS_Y = 32;

class WorldChunk {
  public:
    WorldChunk();

    const std::vector<Block>& getBlocks() const;

  private:
  	std::vector<Block> m_blocks{};
};

#endif // WORLDCHUNK_H_INCLUDED
