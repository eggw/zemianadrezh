#ifndef WORLDCHUNK_H_INCLUDED
#define WORLDCHUNK_H_INCLUDED

#include "Block.h"

#include <vector>

constexpr int CHUNK_DIMENSIONS_X = 32;
constexpr int CHUNK_DIMENSIONS_Y = 32;

class WorldChunk {
	public:
		WorldChunk(int _id, bool _randomiseVisibility = false);

		int getID() const;

		const std::vector<Block>& getBlocks() const;
		void setBlockType(int _id, BlockData::Type _t);

	private:
		int m_id;
		std::vector<Block> m_blocks{};
};

#endif // WORLDCHUNK_H_INCLUDED
