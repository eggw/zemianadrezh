#include "WorldChunk.h"

WorldChunk::WorldChunk() {
    for(int y{0}; y < CHUNK_DIMENSIONS_Y; y++) {
        for(int x{0}; x < CHUNK_DIMENSIONS_X; x++) {
            Block block;
            block.m_positionX = x;
            block.m_positionY = y;

            m_blocks.push_back(block);
        }
    }
}

const std::vector<Block>& WorldChunk::getBlocks() const {
	return m_blocks;
}
