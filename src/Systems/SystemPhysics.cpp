#include "SystemPhysics.h"

#include "Components/ComponentPosition.h"
#include "Components/ComponentPhysics.h"

SystemPhysics::SystemPhysics() {
}

//Iterate through all entites with Position and Physics components,
//and adjust their position appropriately depending on their velocity
void SystemPhysics::update(int _timeslice, entt::registry& _reg) {
	auto view = _reg.view<ComponentPosition, ComponentPhysics>();
	for (auto& entity : view) {
		auto&      pos = view.get<ComponentPosition>(entity).m_position;
		const auto vel = view.get<ComponentPhysics>(entity).m_velocity;
		pos = {(pos.x + vel.x * _timeslice), (pos.y + vel.y * _timeslice)};
	}
}
