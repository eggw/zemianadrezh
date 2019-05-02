#ifndef GAMEINSTANCE_H_INCLUDED
#define GAMEINSTANCE_H_INCLUDED

#include <memory>
#include <entt/entity/registry.hpp>

#include "World.h"
#include "Player.h"

#include "Components/ComponentsPlayer.h"

class GameInstance {
	friend class ProgramState_Play;

  public:
	GameInstance();

	const World& getWorld() const;
	//const std::shared_ptr<std::vector<std::shared_ptr<Player>>> getPlayers()
	// const;

	const encodedChunks encodeWorldChunks() const;
	void parseWorldChunk(const WorldChunk::EncodedChunkData& _data);


	//Checks if a player with the name given in _data exists, and if so, updates
	//their components. Otherwise, calls addPlayer().
    void updatePlayer(const ComponentsPlayer& _data);
    //Returns the entity ID of the player with a certain name
    entt::entity getPlayer(std::string& _name);

	void removePlayer(entt::entity _e);
	void removePlayer(std::string& _name);

  protected:
	World m_world;
	//TODO
	//The reason we [should?] use a shared pointer here is because in the case
	//of a local server, we're going to bypass packet communication completely
	//and simply assign the client's resources to the memory address of the
	//server's.

	//The registry that will contain all our entities
	entt::registry m_registry;
  private:
	void addPlayer(const ComponentsPlayer& _data);
};

#endif // GAMEINSTANCE_H_INCLUDED
