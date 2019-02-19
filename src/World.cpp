#include "World.h"

#include <SFML/Network.hpp>

//Packet operator overloading
//----------------------------------------------------------------------------------------------------------------
sf::Packet& operator <<(sf::Packet& _p, const World::EncodedWorldData& _d) {
	return _p << _d.chunkIDs << _d.blocks;
}

sf::Packet& operator >>(sf::Packet& _p, World::EncodedWorldData& _d) {
	return _p >> _d.chunkIDs >> _d.blocks;
}
//----------------------------------------------------------------------------------------------------------------

World::World() {

}

const sf::Vector2f World::getCenter() const {
	int index{0};
	float xPos{0};
	float yPos{0};

	if(m_chunks.size() == 0) {
		return {0,0};
	}
	else if(m_chunks.size() % 2 == 0) {
		index = m_chunks.size() / 2;
		xPos = index * BLOCK_DIMENSIONS_X * CHUNK_DIMENSIONS_X;
		yPos = 300;
	}
	else {
		index = m_chunks.size() / 2;
		xPos = index * BLOCK_DIMENSIONS_X * CHUNK_DIMENSIONS_X;
		xPos += (CHUNK_DIMENSIONS_X * BLOCK_DIMENSIONS_X) / 2;
		yPos = 300;
	}

	return {xPos,yPos};
}

void World::addChunk(int _num, bool _empty) {
	for(int i{0}; i < _num; i++) {
		WorldChunk w{ int(m_chunks.size()), _empty};
		m_chunks.push_back(w);
	}
}

const std::vector<WorldChunk>& World::getChunks() const {
	return m_chunks;
}

const World::EncodedWorldData World::encodeData() const {
	/*
	chunkIDs will contain the IDs of each chunk, delimited
	by % (ex: 0%1%2%3%4)

	blocks will contain both the type (BlockData::Type enum)
	and position of all non-air (non-0) blocks in the chunk,
	with the block types delimited by , and positions delimited
	by .
	For example:

	1,14.1,156.1,332.1,745.1,1020%1,599.1,766.1,923.1,1004%...

	all the blocks before the first % symbol would correspond to the
	chunk with the ID 0, the blocks after the symbol to the chunk with
	ID 1, and so on. All the blocks in the example are of type 1, which
	corresponds to BlockData::Type::DIRT, so they will be dirt blocks.
	*/

	EncodedWorldData data;

	for(size_t i{0}; i < m_chunks.size(); i++) {
		data.chunkIDs += std::to_string(m_chunks[i].getID());
		data.chunkIDs += "%";

		auto blocks = m_chunks[i].getBlocks();

		for(size_t z{0}; z < blocks.size(); z++) {
			int type {blocks[z].getType()};
			if(type != 0) {
				data.blocks += std::to_string(type);
				data.blocks += ",";
				data.blocks += std::to_string(z);
				data.blocks += ".";
			}
		}

		data.blocks += "%";

	}

	return data;
}

void World::parseData(const World::EncodedWorldData& _data) {
	m_chunks.clear();
	std::vector<int> chunkIDs;

	std::string currentNumber;

	for(const char& c : _data.chunkIDs) {
		if(c != '%') {
			currentNumber += c;
		}
		else {
			int id = std::stoi(currentNumber);
			currentNumber.clear();
			m_chunks.push_back({id, true});
			chunkIDs.push_back(id);
		}
	}

	currentNumber.clear();

	if(chunkIDs.empty()) {
		return;
	}

	auto it = chunkIDs.begin();
	int currentChunkID{*it};

	int currentType;
	std::string currentTypeStr{""};
	bool readingType{true};

	for(const char& c : _data.blocks) {
		if(c == ',') {
			readingType = false;
			currentType = std::stoi(currentTypeStr);
			currentTypeStr.clear();
		}

		else if(c == '.') {
			int id = std::stoi(currentNumber);
			currentNumber.clear();
			m_chunks[currentChunkID].setBlockType(id, BlockData::Type(currentType));
			readingType = true;
		}

		else if(c == '%') {
			if(it != chunkIDs.end() - 1) {
				it++;
				currentChunkID = *it;
			}
		}

		else if(readingType) {
			currentTypeStr += c;
		}

		else if(!readingType) {
			currentNumber += c;
		}
	}
}

/*
This is useful for debugging inconsistencies between encoded/decoded
world data, just keeping it as a comment in case it's ever needed
again
#include <fstream>
#include <map>
#include <string>
#include <iostream>
void tempLog(std::string _str) {
	std::ofstream output;
	output.open("log_world.txt", std::fstream::app);
	output << _str;
	output << "\n";
	output.close();
	return;
}
*/
