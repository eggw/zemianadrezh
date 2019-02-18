#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include <string>

#include "WorldChunk.h"

class World {
	public:
		struct EncodedWorldData {
			std::string chunkIDs;
			std::string invisibleBlocks;
		};

		World();

		const sf::Vector2f getCenter() const;

		void addChunk(int _num = 1, bool _randomiseVisibility = false);

		const std::vector<WorldChunk>& getChunks() const;
		const EncodedWorldData encodeData() const;
		void parseData(const EncodedWorldData& _data);
	private:
		std::vector<WorldChunk> m_chunks;
};


//Forward declare Packet here and define the overloading in World.cpp, so
//we don't have to include SFML/Networking.hpp in World.h
namespace sf {
	class Packet;
}
sf::Packet& operator <<(sf::Packet& _p, const World::EncodedWorldData& _d);
sf::Packet& operator >>(sf::Packet& _p, World::EncodedWorldData& _d);
#endif // WORLD_H_INCLUDED
