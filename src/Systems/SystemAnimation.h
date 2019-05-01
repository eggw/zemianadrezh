#ifndef SYSTEMANIMATION_H_INCLUDED
#define SYSTEMANIMATION_H_INCLUDED

#include <entt/entity/registry.hpp>

class SystemAnimation {
  public:
	SystemAnimation();
	void update(int _timeslice, entt::registry& _reg);
};

#endif // SYSTEMPHYSICS_H_INCLUDED
