#include "SystemPhysics.h"

#include "Components/ComponentPosition.h"
#include "Components/ComponentPhysics.h"

SystemPhysics::SystemPhysics(int _left, int _right)
            : m_leftmostPoint{_left}, m_rightmostPoint{_right} {
}

//Iterate through all entites with Position and Physics components,
//and adjust their position appropriately depending on their velocity
void SystemPhysics::update(int _timeslice, entt::registry& _reg) {
	auto view = _reg.view<ComponentPosition, ComponentPhysics>();
	for (auto& entity : view) {
		auto&      pos = view.get<ComponentPosition>(entity).m_position;
		const auto vel = view.get<ComponentPhysics>(entity).m_velocity;

		if (pos.x <= m_leftmostPoint && vel.x < 0) {
			pos.x = m_leftmostPoint;
			return;
		}
		else if (pos.x >= m_rightmostPoint && vel.x > 0) {
			pos.x = m_rightmostPoint;
		}

		pos = {(pos.x + vel.x * _timeslice), (pos.y + vel.y * _timeslice)};
	}
}
