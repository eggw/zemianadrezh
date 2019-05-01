#include "SystemPlayerMovement.h"

#include <SFML/Window/Keyboard.hpp>

#include "Keybinds.h"
#include "InputLocker.h"

#include "Components/ComponentPhysics.h"
#include "Components/ComponentDirection.h"

SystemPlayerMovement::SystemPlayerMovement(entt::entity _uid)
            : m_playerID{_uid} {
}

void SystemPlayerMovement::getInput(entt::registry& _reg) {
	if (InputLocker::get_instance().isLocked()) {
		return;
	}

	auto& vel        = _reg.get<ComponentPhysics>(m_playerID).m_velocity;
	auto& facingLeft = _reg.get<ComponentDirection>(m_playerID).m_facingLeft;

	if (sf::Keyboard::isKeyPressed(Key::LEFT_KEY)) {
		facingLeft = true;
		vel.x      = -0.2;
	}

	else if (sf::Keyboard::isKeyPressed(Key::RIGHT_KEY)) {
		facingLeft = false;
		vel.x      = 0.2;
	}
	else {
		vel.x = 0;
	}
}

void SystemPlayerMovement::update(int _timeslice, entt::registry& _reg) {
}
