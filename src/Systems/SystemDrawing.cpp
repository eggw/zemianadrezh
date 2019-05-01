#include "SystemDrawing.h"

#include "Components/ComponentSprite.h"
#include "Components/ComponentAnimation.h"

SystemDrawing::SystemDrawing(sf::RenderWindow& _window) : m_window{_window} {
}

//Iterate through all entites with Sprite components and simply draw them
//to our window

//Also, since Animation components will have their own sprites that vary
//and can be accessed with Animation::getCurrentSprite(), they will be
//drawn separately.
void SystemDrawing::draw(entt::registry& _reg) {
	auto spriteView = _reg.view<ComponentSprite>();
	for (auto& entity : spriteView) {
		auto sprite = spriteView.get(entity).m_sprite;
		m_window.draw(sprite);
	}
	auto animationView = _reg.view<ComponentAnimation>();
	for (auto& entity : animationView) {
		auto sprite = animationView.get(entity).m_animation.getCurrentSprite();
		m_window.draw(sprite);
	}
}
