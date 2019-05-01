#ifndef COMPONENTSPRITE_H_INCLUDED
#define COMPONENTSPRITE_H_INCLUDED

#include <SFML/Graphics.hpp>

struct ComponentSprite {
  public:
	ComponentSprite(sf::Sprite& _s) : m_sprite{_s} {};

	//Move assignment overload
	ComponentSprite& operator=(const ComponentSprite&) noexcept {
		return *this;
	};

	sf::Sprite m_sprite;
};

#endif // COMPONENTSPRITE_H_INCLUDED
