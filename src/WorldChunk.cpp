#include "WorldChunk.h"

WorldChunk::WorldChunk(int _id)
:	m_id{_id} {
    for(int y{0}; y < CHUNK_DIMENSIONS_Y; y++) {
        for(int x{0}; x < CHUNK_DIMENSIONS_X; x++) {
            Block block;
            block.m_positionX = x;
            block.m_positionY = y;

            m_blocks.push_back(block);
        }
    }
}

int WorldChunk::getID() const{
	return m_id;
}

const std::vector<Block>& WorldChunk::getBlocks() const {
	return m_blocks;
}
