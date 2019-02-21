#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include "WorldChunk.h"

typedef std::vector<WorldChunk::EncodedChunkData> encodedChunks;

class World {
	public:
		World();

		const sf::Vector2f getCenter() const;
		void addChunk(int _beginID, int _num, bool _empty);
		const std::vector<WorldChunk>& getChunks() const;

		const encodedChunks encodeChunks() const;
		void parseChunk(const WorldChunk::EncodedChunkData& _data);
	private:
		std::vector<WorldChunk> m_chunks;
};

#endif // WORLD_H_INCLUDED
