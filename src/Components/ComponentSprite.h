#ifndef COMPONENTSPRITE_H_INCLUDED
#define COMPONENTSPRITE_H_INCLUDED

#include <SFML/Graphics.hpp>

struct ComponentSprite {
  public:
	ComponentSprite(sf::Sprite& _s) : m_sprite{_s} {};

	sf::Sprite m_sprite;
};

#endif // COMPONENTSPRITE_H_INCLUDED
