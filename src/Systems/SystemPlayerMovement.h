#ifndef SYSTEMPLAYERMOVEMENT_H_INCLUDED
#define SYSTEMPLAYERMOVEMENT_H_INCLUDED

#include <entt/entity/registry.hpp>

class SystemPlayerMovement {
  public:
	SystemPlayerMovement();
	void getInput(entt::registry& _reg, entt::entity _uid);
};

#endif // SYSTEMPHYSICS_H_INCLUDED
