#include "World.h"

#include <iostream>

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

void World::addChunk(int _beginID, int _num, bool _empty) {
	for(int i{0}; i < _num; i++) {
		WorldChunk w{ _beginID, _empty};
		m_chunks.push_back(w);
		++_beginID;
	}
}

const std::vector<WorldChunk>& World::getChunks() const {
	return m_chunks;
}


const encodedChunks World::encodeChunks() const {
	encodedChunks data;

	for(auto& chunk : m_chunks) {
		data.emplace_back(chunk.encodeData());
	}

	return data;
}


void World::parseChunk(const WorldChunk::EncodedChunkData& _data) {
	for(auto& chunk : m_chunks) {
		if(chunk.getID() == _data.id) {
			chunk.parseData(_data);
			return;
		}
	}

	//If a chunk with the corresponding ID was not found, we'll
	//create it:
	WorldChunk newChunk{_data.id, true};
	newChunk.parseData(_data);
	m_chunks.emplace_back(newChunk);
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
