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
#include "Util/Coordinates.h"

#include <iostream>
World::World()
            : m_chunks{},
              m_rendererChunk{std::make_unique<RendererChunk>(*this)},
              m_time{{5, 0}},
              m_dayNightCycle{} {
}

World::~World() {
}


void World::initialise() {
	for(int i{0}; i < WORLD_DIMENSIONS_X * WORLD_DIMENSIONS_Y; i++) {
		addChunk({i, false});
	}
	assignNeighbors();
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
			assignNeighbors();
			return;
		}
	}

	//If a chunk with the corresponding ID was not found, we'll
	//create it:
	WorldChunk newChunk{_data.id, true};
	newChunk.parseData(_data);

	addChunk(newChunk);
	assignNeighbors();
}

void World::update(int _timeslice) {
	m_time.tick();
	m_dayNightCycle.update(m_time);
}

void World::drawBackground(sf::RenderTarget& target,
                           sf::RenderStates  states) const {
	target.draw(m_dayNightCycle, states);
}

void World::drawChunks(sf::RenderTarget& target,
                       sf::RenderStates  states) const {
	m_rendererChunk->draw(target, states);
}

void World::renderUpdatedChunk(int _chunkID) const {
	m_rendererChunk->update(_chunkID);
}

void World::addChunk(WorldChunk _chunk) {
	m_chunks.push_back(_chunk);
}

void World::assignNeighbors() {
	for(auto& chunk : m_chunks) {
		static const sf::Vector2i dim{WORLD_DIMENSIONS_X, WORLD_DIMENSIONS_Y};
		const auto position{chunk.getPosition()};

		WorldChunk::NeighboringChunks neighbors;
		neighbors.insert({Direction::NORTH, nullptr});
		neighbors.insert({Direction::SOUTH, nullptr});
		neighbors.insert({Direction::EAST, nullptr});
		neighbors.insert({Direction::WEST, nullptr});

		auto northPosition{Utility::Coordinates::northOf(position)};
		auto southPosition{Utility::Coordinates::southOf(position)};
		auto eastPosition{Utility::Coordinates::eastOf(position)};
		auto westPosition{Utility::Coordinates::westOf(position)};

		if (!Utility::Coordinates::outOfRange(northPosition, dim)) {
			int i{Utility::Coordinates::getIndex(northPosition, dim.x)};
			neighbors[Direction::NORTH] = &m_chunks[i];
		}

		if (!Utility::Coordinates::outOfRange(southPosition, dim)) {
			int i{Utility::Coordinates::getIndex(southPosition, dim.x)};
			neighbors[Direction::SOUTH] = &m_chunks[i];
		}

		if (!Utility::Coordinates::outOfRange(eastPosition, dim)) {
			int i{Utility::Coordinates::getIndex(eastPosition, dim.x)};
			neighbors[Direction::EAST] = &m_chunks[i];

		}

		if (!Utility::Coordinates::outOfRange(westPosition, dim)) {
			int i{Utility::Coordinates::getIndex(westPosition, dim.x)};
			neighbors[Direction::WEST] = &m_chunks[i];
		}

		chunk.assignNeighbors(neighbors);
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------TESTS-------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Testing world unparsing/reparsing") {
	World world{};
	world.initialise();
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
