#ifndef SYSTEMPHYSICS_H_INCLUDED
#define SYSTEMPHYSICS_H_INCLUDED

#include <SFML/System/Vector2.hpp>
#include <entt/entity/registry.hpp>

class SystemPhysics {
  public:
	SystemPhysics();
	void update(int _timeslice, entt::registry& _reg);
};

#endif // SYSTEMPHYSICS_H_INCLUDED
