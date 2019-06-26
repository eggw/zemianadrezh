//////////////////////////////////////////////|
//Contains tests at the end of the file       |
//////////////////////////////////////////////|
#include "debug.h"
//--------------------------------------------|
// (*) World parsing/unparsing
//--------------------------------------------|
#include "Constants.h"

#include "World.h"

#include "RendererChunk.h"
#include "TextureManager.h"
#include "Util/Coordinates.h"

World::World()
            : m_chunks{},
              m_rendererChunk{std::make_unique<RendererChunk>(*this)},
              m_time{{7, 0}},
              m_dayNightCycle{} {
}

World::~World() {
}

void World::initialise() {
	for (int i{0}; i < Dimensions::World::X * Dimensions::World::Y; i++) {
		addChunk({i, false});
	}
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
		xPos  = index * Dimensions::Block::X * Dimensions::Chunk::X;
		yPos  = 300;
	}
	else {
		index = m_chunks.size() / 2;
		xPos  = index * Dimensions::Block::X * Dimensions::Chunk::X;
		xPos += (Dimensions::Chunk::X * Dimensions::Block::X) / 2;
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
			updateChunkNeighbors(&chunk);
			return;
		}
	}

	//If a chunk with the corresponding ID was not found, we'll
	//create it:
	WorldChunk newChunk{_data.id, true};
	newChunk.parseData(_data);
	addChunk(newChunk);
}

void World::tickTime(int _timeslice) {
	m_time.tick();
}

void World::update(int _timeslice) {
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

HHMM World::getTime() const {
	return m_time.get();
}

void World::setTime(HHMM _t) {
	m_time.set(_t);
}

void World::addChunk(WorldChunk _chunk) {
	m_chunks.push_back(_chunk);
	updateChunkNeighbors(&m_chunks.back());
}

void World::updateChunkNeighbors(WorldChunk* _chunk, bool _recurse) {
	static const sf::Vector2i dim{Dimensions::World::X, Dimensions::World::Y};
	const auto                position{_chunk->getPosition()};

	WorldChunk::NeighboringChunks neighbors;
	neighbors.insert({Direction::NORTH, nullptr});
	neighbors.insert({Direction::SOUTH, nullptr});
	neighbors.insert({Direction::EAST, nullptr});
	neighbors.insert({Direction::WEST, nullptr});

	auto northPosition{Utility::Coordinates::northOf(position)};
	auto southPosition{Utility::Coordinates::southOf(position)};
	auto eastPosition{Utility::Coordinates::eastOf(position)};
	auto westPosition{Utility::Coordinates::westOf(position)};

	std::vector<WorldChunk*> toRecurseOver{};

	int chunksSize {int(m_chunks.size())};

	if (!Utility::Coordinates::outOfRange(northPosition, dim)) {
		int i{Utility::Coordinates::getIndex(northPosition, dim.x)};
		if (i < chunksSize) {
			neighbors[Direction::NORTH] = &m_chunks[i];
			toRecurseOver.push_back(&m_chunks[i]);
		}
	}

	if (!Utility::Coordinates::outOfRange(southPosition, dim)) {
		int i{Utility::Coordinates::getIndex(southPosition, dim.x)};
		if (i < chunksSize) {
			neighbors[Direction::SOUTH] = &m_chunks[i];
			toRecurseOver.push_back(&m_chunks[i]);
		}
	}

	if (!Utility::Coordinates::outOfRange(eastPosition, dim)) {
		int i{Utility::Coordinates::getIndex(eastPosition, dim.x)};
		if (i < chunksSize) {
			neighbors[Direction::EAST] = &m_chunks[i];
			toRecurseOver.push_back(&m_chunks[i]);
		}
	}

	if (!Utility::Coordinates::outOfRange(westPosition, dim)) {
		int i{Utility::Coordinates::getIndex(westPosition, dim.x)};
		if (i < chunksSize) {
			neighbors[Direction::WEST] = &m_chunks[i];
			toRecurseOver.push_back(&m_chunks[i]);
		}
	}

	_chunk->assignNeighbors(neighbors);
	m_rendererChunk->update(_chunk->getID());

	if (_recurse) {
		for (auto& chunk : toRecurseOver) {
			updateChunkNeighbors(chunk, false);
		}
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
