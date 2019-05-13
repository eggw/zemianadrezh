#ifndef GAMEINSTANCE_H_INCLUDED
#define GAMEINSTANCE_H_INCLUDED

#include <memory>
#include <entt/entity/registry.hpp>

#include "World.h"
#include "Components/ComponentsPlayer.h"

class GameInstance {
  public:
	GameInstance(GameInstance* _g = nullptr);

	//TODO: refactor these functions, either remove them or put them somewhere else
	//-------------------------------------------------------------------------
	const EncodedChunks encodeWorldChunks() const;
	void parseWorldChunk(const WorldChunk::EncodedChunkData& _data);
	//-------------------------------------------------------------------------

	//Checks if a player with the name given in _data exists, in the entity
	//registry, and if so, updates their components. Otherwise, calls addPlayer().
	void updatePlayer(const ComponentsPlayer& _data);

	//Removes a player from the entity registry
	void removePlayer(entt::entity _e);
	void removePlayer(std::string& _name);

  protected:
	//Functions -----------------------------------
	void addPlayer(const ComponentsPlayer& _data);
	void addPlayer(const ComponentsPlayer& _data, entt::entity _e);
	//If given a pointer to another GameInstance _g, we'll share _g's world and
	//registry. This is how a client can share resources with a local server.
	void initialise(GameInstance* _g);
	//If not given any parameters, we'll allocate memory for our world and registry.
	//This is what a server should call to initialise itself.
	void initialise();
	//---------------------------------------------

	//Data members --------------------------------
	//The reason we use shared pointers here is so that a client can share resources
	//with a local server.
	std::shared_ptr<World>          m_world;
	std::shared_ptr<entt::registry> m_registry;
	//---------------------------------------------
};

#endif // GAMEINSTANCE_H_INCLUDED
