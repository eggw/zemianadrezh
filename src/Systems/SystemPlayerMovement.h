#ifndef SYSTEMPLAYERMOVEMENT_H_INCLUDED
#define SYSTEMPLAYERMOVEMENT_H_INCLUDED

#include <entt/entity/registry.hpp>

class SystemPlayerMovement {
  public:
	//The Player Movement System takes in the entity ID of
	//the player in order to know which entity to deal with.
	SystemPlayerMovement(entt::entity _uid);
	void getInput(entt::registry& _reg);
	void update(int _timeslice, entt::registry& _reg);

  private:
	entt::entity m_playerID;
};

#endif // SYSTEMPHYSICS_H_INCLUDED
