#include "WorldChunk.h"

#include <random>
#include <chrono>

#include <SFML/Network.hpp>
#include "Util/Coordinates.h"

//Packet operator overloading
//----------------------------------------------------------------------------------------------------------------
sf::Packet& operator<<(sf::Packet& _p, const WorldChunk::EncodedChunkData& _d) {
	return _p << _d.id << _d.blocks;
}

sf::Packet& operator>>(sf::Packet& _p, WorldChunk::EncodedChunkData& _d) {
	return _p >> _d.id >> _d.blocks;
}
//----------------------------------------------------------------------------------------------------------------

WorldChunk::WorldChunk(int _id, bool _empty) : m_id{_id}, m_blocks{}, m_neighbors{} {
	m_neighbors.insert({Direction::NORTH, nullptr});
	m_neighbors.insert({Direction::SOUTH, nullptr});
	m_neighbors.insert({Direction::EAST, nullptr});
	m_neighbors.insert({Direction::WEST, nullptr});
	//index represents a block's position in the chunk.
	//First, the x axis is filled out, then the y, so
	//it looks like this:

	int index{0};

	for (int y{0}; y < CHUNK_DIMENSIONS_Y; y++) {
		for (int x{0}; x < CHUNK_DIMENSIONS_X; x++) {
			BlockData::Type t{BlockData::Type::AIR};

			if(!_empty) {
				auto seed = std::chrono::high_resolution_clock::now()
				              .time_since_epoch()
				              .count();
				static std::mt19937                mt_rand(seed);
				std::uniform_int_distribution<int> uniform_dist(1, 20);
				int                                mean = uniform_dist(mt_rand);

				if (mean != 1) {
					if(y > 0) {
						t = BlockData::Type::DIRT;
					}
					else {
						t = BlockData::Type::GRASS;
					}
				}
			}

			Block block{index, t};
			m_blocks.push_back(block);
			++index;
		}
	}
}

int WorldChunk::getID() const {
	return m_id;
}

sf::Vector2i WorldChunk::getPosition() const {
	return {Utility::Coordinates::getCoords(m_id, WORLD_DIMENSIONS_X)};
}

sf::Vector2f WorldChunk::getPixelPosition() const {
	return {float(getPosition().x * CHUNK_DIMENSIONS_X * BLOCK_DIMENSIONS_X),
	        float(getPosition().y * CHUNK_DIMENSIONS_Y * BLOCK_DIMENSIONS_Y)};
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

void WorldChunk::assignNeighbors(NeighboringChunks& _neighbors) {
	m_neighbors = _neighbors;
	adjustBorders();
}

//This function returns pointers to a block's neighbors (N,E,S,W).
//
//The first and last rows and columns also take into account the
//neighboring chunks.

Block::NeighboringBlocks WorldChunk::getNeighboringBlocks(Block* _b) {
	static const sf::Vector2i dim{CHUNK_DIMENSIONS_X, CHUNK_DIMENSIONS_Y};

	Block::NeighboringBlocks  result{};
	result.insert({Direction::NORTH, nullptr});
	result.insert({Direction::SOUTH, nullptr});
	result.insert({Direction::EAST, nullptr});
	result.insert({Direction::WEST, nullptr});

	auto position{_b->getPosition()};

	auto         northPosition{Utility::Coordinates::northOf(position)};
	WorldChunk*  northChunk{this};

	auto         southPosition{Utility::Coordinates::southOf(position)};
	WorldChunk*  southChunk{this};

	auto         eastPosition{Utility::Coordinates::eastOf(position)};
	WorldChunk*  eastChunk{this};

	auto         westPosition{Utility::Coordinates::westOf(position)};
	WorldChunk*  westChunk{this};

	if(position.y == 0 && m_neighbors[Direction::NORTH] != nullptr) {
		northChunk = m_neighbors[Direction::NORTH];
		northPosition.y = CHUNK_DIMENSIONS_Y - 1;
	}

	else if(position.y == CHUNK_DIMENSIONS_Y - 1 && m_neighbors[Direction::SOUTH] != nullptr) {
		southChunk = m_neighbors[Direction::SOUTH];
		southPosition.y = 0;
	}

	if(position.x == 0 && m_neighbors[Direction::WEST] != nullptr) {
		westChunk = m_neighbors[Direction::WEST];
		westPosition.x = CHUNK_DIMENSIONS_X - 1;
	}

	else if(position.x == CHUNK_DIMENSIONS_X - 1 && m_neighbors[Direction::EAST] != nullptr) {
		eastChunk = m_neighbors[Direction::EAST];
		eastPosition.x = 0;
	}

	if (!Utility::Coordinates::outOfRange(northPosition, dim)) {
		int i{Utility::Coordinates::getIndex(northPosition, dim.x)};
		if (northChunk->m_blocks[i].getType() != BlockData::Type::AIR) {
			result[Direction::NORTH] = &northChunk->m_blocks[i];
		}
	}

	if (!Utility::Coordinates::outOfRange(southPosition, dim)) {
		int i{Utility::Coordinates::getIndex(southPosition, dim.x)};
		if (southChunk->m_blocks[i].getType() != BlockData::Type::AIR) {
			result[Direction::SOUTH] = &southChunk->m_blocks[i];
		}
	}

	if (!Utility::Coordinates::outOfRange(eastPosition, dim)) {
		int i{Utility::Coordinates::getIndex(eastPosition, dim.x)};
		if (eastChunk->m_blocks[i].getType() != BlockData::Type::AIR) {
			result[Direction::EAST] = &eastChunk->m_blocks[i];
		}
	}

	if (!Utility::Coordinates::outOfRange(westPosition, dim)) {
		int i{Utility::Coordinates::getIndex(westPosition, dim.x)};
		if (westChunk->m_blocks[i].getType() != BlockData::Type::AIR) {
			result[Direction::WEST] = &westChunk->m_blocks[i];
		}
	}

	return result;
}

void WorldChunk::adjustBorders() {
	for (auto& block : m_blocks) {
		if (block.getType() == BlockData::Type::AIR) {
			continue;
		}
		auto neighbors{getNeighboringBlocks(&block)};
		block.adjustBorders(neighbors);
	}
}
