#include "SystemDrawing.h"

#include "Components/ComponentSprite.h"

SystemDrawing::SystemDrawing(sf::RenderWindow& _window) : m_window{_window} {
}

//Iterate through all entites with Sprite components and simply draw them
//to our window
void SystemDrawing::update(int _timeslice, entt::registry& _reg) {
	auto view = _reg.view<ComponentSprite>();
	for (auto& entity : view) {
		auto sprite = view.get(entity).m_sprite;
		m_window.draw(sprite);
	}
}
