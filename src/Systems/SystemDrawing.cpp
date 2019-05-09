#include "SystemDrawing.h"

#include "Components/ComponentSprite.h"
#include "Components/ComponentAnimation.h"
#include "Components/ComponentPosition.h"

SystemDrawing::SystemDrawing(sf::RenderWindow& _window) : m_window{_window} {
}

//This function will iterate through all our sprite components, set their
//position if they have a position component, and then draw the resulting
//sprite. It will do the same for animation components.

void SystemDrawing::draw(entt::registry& _reg) {
	auto spriteView = _reg.view<ComponentSprite>();
	for (auto& entity : spriteView) {
		auto sprite = spriteView.get(entity).m_sprite;

		if (_reg.has<ComponentPosition>(entity)) {
			sprite.setPosition(_reg.get<ComponentPosition>(entity).m_position);
		}

		m_window.draw(sprite);
	}
	auto animationView = _reg.view<ComponentAnimation>();
	for (auto& entity : animationView) {
		auto sprite = animationView.get(entity).m_animation.getCurrentSprite();

		if (_reg.has<ComponentPosition>(entity)) {
			sprite.setPosition(_reg.get<ComponentPosition>(entity).m_position);
		}

		m_window.draw(sprite);
	}
}
