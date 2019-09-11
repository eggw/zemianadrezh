#ifndef WORLDCHUNK_H_INCLUDED
#define WORLDCHUNK_H_INCLUDED

#include <vector>
#include <map>

#include "Block.h"
#include "Util/Coordinates.h"

class WorldChunk {
  public:
	using NeighboringChunks = std::map<Utility::Coordinates::DIRECTION, WorldChunk*>;

	struct EncodedChunkData {
		EncodedChunkData(){};
		sf::Uint16  id;
		std::string blocks;
	};

	WorldChunk(int _id, bool _empty);

	int getID() const;

	//returns the chunk's coordinates in the world
	//(0,0) to (World::X, World::Y)
	sf::Vector2i getPosition() const;
	//returns the top-left corner of the chunk in the world
	//(equivalent to getPosition() * CHUNK_DIMENSIONS * BLOCK_DIMENSIONS)
	sf::Vector2f getPixelPosition() const;

	const std::vector<Block>& getBlocks() const;
	void                      setBlockType(int _id, BlockData::Type _t);

	const EncodedChunkData encodeData() const;
	void                   parseData(const EncodedChunkData& _data);

	//Once this function is called and the chunk knows its
	//neighbors, adjustBorders() is called
	void assignNeighbors(NeighboringChunks& _neighbors);

  private:
	//Functions -----------------------------------
	void                     adjustBorders();
	Block::NeighboringBlocks getNeighboringBlocks(Block* _b);
	//---------------------------------------------

	//Data members --------------------------------
	int                m_id;
	std::vector<Block> m_blocks;
	NeighboringChunks  m_neighbors;
	//---------------------------------------------
};

//Forward declare Packet here and define the overloading in WorldChunk.cpp, so
//we don't have to include SFML/Networking.hpp in WorldChunk.h
namespace sf {
	class Packet;
}
sf::Packet& operator<<(sf::Packet& _p, const WorldChunk::EncodedChunkData& _d);
sf::Packet& operator>>(sf::Packet& _p, WorldChunk::EncodedChunkData& _d);

#endif // WORLDCHUNK_H_INCLUDED
