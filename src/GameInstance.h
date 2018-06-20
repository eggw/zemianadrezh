#ifndef GAMEINSTANCE_H_INCLUDED
#define GAMEINSTANCE_H_INCLUDED

#include <memory>

#include "World.h"
#include "Player.h"

class GameInstance {
	public:
		GameInstance();

		const World& getWorld() const;
		const std::shared_ptr<std::vector<std::unique_ptr<Player>>> getOtherPlayers() const;

		const World::EncodedWorldData encodeWorldData() const;
		void parseWorldData(World::EncodedWorldData& _data);

		virtual void update();

		//Server and Client both have different implementations of this.
		//The server either adds or updates the player data, depending on whether
		//it is already present in m_otherPlayers or not.

		//Client does the same thing, but also does an extra check: if the player
		//being added has the same name as theirs (i.e. it is theirs), it is not
		//appended to m_otherPlayers.
		virtual void updateOtherPlayers(Player::EncodedPlayerData _data) = 0;

	protected:
		World m_world;
		//The reason we use a shared pointer here is because in the case
		//of a local server, we're going to bypass packet communication completely
		//and simply assign the client's resources to the memory address of the
		//server's.
		std::shared_ptr<std::vector<std::unique_ptr<Player>>> m_otherPlayers;
};

#endif // GAMEINSTANCE_H_INCLUDED
