#include "WorldChunk.h"

#include <random>
#include <chrono>

#include <SFML/Network.hpp>

//Packet operator overloading
//----------------------------------------------------------------------------------------------------------------
sf::Packet& operator<<(sf::Packet& _p, const WorldChunk::EncodedChunkData& _d) {
	return _p << _d.id << _d.blocks;
}

sf::Packet& operator>>(sf::Packet& _p, WorldChunk::EncodedChunkData& _d) {
	return _p >> _d.id >> _d.blocks;
}
//----------------------------------------------------------------------------------------------------------------

WorldChunk::WorldChunk(int _id, bool _empty) : m_id{_id} {
	//index represents a block's position in the chunk.
	//First, the x axis is filled out, then the y, so
	//it looks like this:

	int index{0};

	for (int y{0}; y < CHUNK_DIMENSIONS_Y; y++) {
		for (int x{0}; x < CHUNK_DIMENSIONS_X; x++) {
			bool air{false};

			if (_empty) {
				air = true;
			}

			else {
				auto seed = std::chrono::high_resolution_clock::now()
				              .time_since_epoch()
				              .count();
				static std::mt19937                mt_rand(seed);
				std::uniform_int_distribution<int> uniform_dist(1, 20);
				int                                mean = uniform_dist(mt_rand);

				if (mean == 1) {
					air = true;
				}
			}

			BlockData::Type t;
			if (!air) {
				t = BlockData::Type::DIRT;
			}
			else {
				t = BlockData::Type::AIR;
			}

			Block block{index, t};
			block.setPosition({float(x), float(y)});

			++index;

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

void WorldChunk::setBlockType(int _id, BlockData::Type _t) {
	m_blocks[_id].setType(_t);
}

const WorldChunk::EncodedChunkData WorldChunk::encodeData() const {
	/*
	chunkID will contain the chunk's ID

	blocks will contain both the type (BlockData::Type enum)
	and position of all non-air (non-0) blocks in the chunk,
	with the block types delimited by , and positions delimited
	by .
	For example:

	1,14.1,156.1,332.1,745.1,1020

	All the blocks in the example are of type 1, which
	corresponds to BlockData::Type::DIRT, so they will be dirt blocks.
	*/

	EncodedChunkData data;

	data.id = sf::Uint16(m_id);

	auto blocks = getBlocks();

	for (size_t z{0}; z < blocks.size(); z++) {
		int type{blocks[z].getType()};
		if (type != 0) {
			data.blocks += std::to_string(type);
			data.blocks += ",";
			data.blocks += std::to_string(z);
			data.blocks += ".";
		}
	}

	return data;
}

void WorldChunk::parseData(const WorldChunk::EncodedChunkData& _data) {
	m_id = _data.id;
	std::string currentNumber;

	int         currentType;
	std::string currentTypeStr{""};
	bool        readingType{true};

	for (const char& c : _data.blocks) {
		if (c == ',') {
			readingType = false;
			currentType = std::stoi(currentTypeStr);
			currentTypeStr.clear();
		}

		else if (c == '.') {
			int id = std::stoi(currentNumber);
			currentNumber.clear();
			setBlockType(id, BlockData::Type(currentType));
			readingType = true;
		}

		else if (readingType) {
			currentTypeStr += c;
		}

		else if (!readingType) {
			currentNumber += c;
		}
	}
}
