#ifndef GAMEINSTANCE_H_INCLUDED
#define GAMEINSTANCE_H_INCLUDED

#include <memory>

#include "World.h"
#include "Player.h"

class GameInstance {
	public:
		GameInstance();

		const World& getWorld() const;
		const std::shared_ptr<std::vector<std::unique_ptr<Player>>> getPlayers() const;

		const World::EncodedWorldData encodeWorldData() const;
		void parseWorldData(World::EncodedWorldData& _data);

		//////////////////////////////////////////////////////////////////////////
		//Server and Client both have different implementations of these functions

		//updatePlayer():
		//The server simply updates the the player contained within m_players
		//that has the same name as the name contained in _data

		//Client does the same thing, but also does an extra check: if the player
		//being updated has the same name as theirs (i.e. it is theirs), nothing
		//is done.
		//Additionally, if the _data.playerName does not match any existing player
		//in m_players, addPlayer() is called.
		virtual void updatePlayer(Player::EncodedPlayerData _data) = 0;

		//addPlayer():
		//The server creates a new player, sets its position to spawn, and appends
		//it to m_players. It only uses _data.playerName and ignores the rest

		//The client creates a new player with all the information contained in _data,
		//and appends it to m_players, if _data.playerName isn't the same as the
		//client's own player
		virtual void addPlayer(Player::EncodedPlayerData _data) = 0;
		//////////////////////////////////////////////////////////////////////////

		void removePlayer(std::string _name);

	protected:
		World m_world;
		//The reason we use a shared pointer here is because in the case
		//of a local server, we're going to bypass packet communication completely
		//and simply assign the client's resources to the memory address of the
		//server's.
		std::shared_ptr<std::vector<std::unique_ptr<Player>>> m_players;
};

#endif // GAMEINSTANCE_H_INCLUDED
