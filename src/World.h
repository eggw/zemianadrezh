#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include "WorldChunk.h"
#include "WorldTime.h"

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

	const WorldTime& getTime() const;

  private:
	//Data members --------------------------------
	std::vector<WorldChunk> m_chunks;
	WorldTime               m_time;
	//---------------------------------------------
};

#endif // WORLD_H_INCLUDED
