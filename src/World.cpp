//////////////////////////////////////////////|
//Contains tests at the end of the file       |
//////////////////////////////////////////////|
#include "debug.h"
//--------------------------------------------|
// (*) World parsing/unparsing
//--------------------------------------------|

#include "World.h"

#include "RendererChunk.h"
#include "TextureManager.h"

World::World() : m_chunks{},
                 m_rendererChunk{std::make_unique<RendererChunk>(*this)},
                 m_time{{5, 0}},
                 m_dayNightCycle{} {
}

World::~World() {
}

const sf::Vector2f World::getCenter() const {
	int   index{0};
	float xPos{0};
	float yPos{0};

	if (m_chunks.size() == 0) {
		return {0, 0};
	}
	else if (m_chunks.size() % 2 == 0) {
		index = m_chunks.size() / 2;
		xPos  = index * BLOCK_DIMENSIONS_X * CHUNK_DIMENSIONS_X;
		yPos  = 300;
	}
	else {
		index = m_chunks.size() / 2;
		xPos  = index * BLOCK_DIMENSIONS_X * CHUNK_DIMENSIONS_X;
		xPos += (CHUNK_DIMENSIONS_X * BLOCK_DIMENSIONS_X) / 2;
		yPos = 300;
	}

	return {xPos, yPos};
}

void World::addChunk(int _beginID, int _num, bool _empty) {
	for (int i{0}; i < _num; i++) {
		WorldChunk w{_beginID, _empty};
		m_chunks.push_back(w);
		++_beginID;
	}
}

const EncodedChunks World::encodeChunks() const {
	EncodedChunks data;

	for (auto& chunk : m_chunks) {
		data.emplace_back(chunk.encodeData());
	}

	return data;
}

void World::parseChunk(const WorldChunk::EncodedChunkData& _data) {
	for (auto& chunk : m_chunks) {
		if (chunk.getID() == _data.id) {
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

void World::update(int _timeslice) {
	m_time.tick();
	m_dayNightCycle.update(m_time);
}

void World::drawBackground(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_dayNightCycle, states);
}

void World::drawChunks(sf::RenderTarget& target, sf::RenderStates states) const {
	m_rendererChunk->draw(target, states);
}

void World::renderUpdatedChunk(int _chunkID) {
	m_rendererChunk->update(_chunkID);
}

/////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------TESTS-------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Testing world unparsing/reparsing") {
	World world{};
	world.addChunk(0, 1, false);
	auto       encodedData{world.encodeChunks().back()};
	const auto toDecode{encodedData};
	world.parseChunk(toDecode);
	auto reEncoded{world.encodeChunks().back()};
	SUBCASE("test if re-encoded world chunks match") {
		CHECK(encodedData.id == reEncoded.id);
		CHECK(encodedData.blocks == reEncoded.blocks);
	}
}
/////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////
