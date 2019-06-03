#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "WorldChunk.h"
#include "WorldTime.h"
#include "DayNightCycle.h"

class RendererChunk;
typedef std::vector<WorldChunk::EncodedChunkData> EncodedChunks;

class World {
	friend class RendererChunk;

  public:
	World();
	~World();

	void initialise();
	const sf::Vector2f getCenter() const;

	const EncodedChunks encodeChunks() const;
	void                parseChunk(const WorldChunk::EncodedChunkData& _data);

	void update(int _timeslice);
	void drawBackground(sf::RenderTarget& target,
	                    sf::RenderStates  states) const;
	void drawChunks(sf::RenderTarget& target, sf::RenderStates states) const;

	void renderUpdatedChunk(int _chunkID) const;

  private:
  	//Functions -----------------------------------
  	void addChunk(WorldChunk _chunk);
  	//assignNeighbors() iterates over all our chunks
  	//and appropriately sets their NESW neighbors.
  	void assignNeighbors();
  	//---------------------------------------------

	//Data members --------------------------------
	std::vector<WorldChunk>        m_chunks;
	std::unique_ptr<RendererChunk> m_rendererChunk;
	WorldTime                      m_time;
	DayNightCycle                  m_dayNightCycle;
	//---------------------------------------------
};

#endif // WORLD_H_INCLUDED
