#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "WorldChunk.h"
#include "WorldTime.h"
#include "DayNightCycle.h"

typedef std::vector<WorldChunk::EncodedChunkData> encodedChunks;

class World {
	friend class RendererChunk;

  public:
	World();

	const sf::Vector2f getCenter() const;
	void               addChunk(int _beginID, int _num, bool _empty);

	const encodedChunks encodeChunks() const;
	void                parseChunk(const WorldChunk::EncodedChunkData& _data);

	void update(int _timeslice);
	void drawDayNightCycle(sf::RenderTarget& target, sf::RenderStates states) const;

  private:
	//Data members --------------------------------
	std::vector<WorldChunk> m_chunks;
	WorldTime               m_time;
	DayNightCycle           m_dayNightCycle;
	//---------------------------------------------
};

#endif // WORLD_H_INCLUDED
