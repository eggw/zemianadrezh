#ifndef SYSTEMPHYSICS_H_INCLUDED
#define SYSTEMPHYSICS_H_INCLUDED

#include <SFML/System/Vector2.hpp>
#include <entt/entity/registry.hpp>

class SystemPhysics {
  public:
	SystemPhysics(int _left, int _right);
	void update(int _timeslice, entt::registry& _reg);

	private:

	const int m_leftmostPoint;
	const int m_rightmostPoint;
};

#endif // SYSTEMPHYSICS_H_INCLUDED
