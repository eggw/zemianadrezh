#include "WorldChunk.h"

#include <random>
#include <chrono>

WorldChunk::WorldChunk(int _id, bool _randomiseVisibility)
	:m_id{_id} {
	for(int y{0}; y < CHUNK_DIMENSIONS_Y; y++) {
		for(int x{0}; x < CHUNK_DIMENSIONS_X; x++) {

			bool visible{true};

			if(_randomiseVisibility) {
				auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
				static std::mt19937 mt_rand(seed);
				std::uniform_int_distribution<int> uniform_dist(1, 20);
				int mean = uniform_dist(mt_rand);

				if(mean == 1) {
					visible = false;
				}
			}

			Block block{visible};
			block.m_position.x = x;
			block.m_position.y = y;

			m_blocks.push_back(block);
		}
	}
}

int WorldChunk::getID() const {
	return m_id;
}

const std::vector<Block>& WorldChunk::getBlocks() const {
	return m_blocks;
}

void WorldChunk::toggleVisibility(int _id, bool _v) {
	m_blocks[_id].setVisibility(_v);
}
