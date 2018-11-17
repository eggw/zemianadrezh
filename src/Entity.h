#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <SFML/System/Vector2.hpp>

struct Entity {
	void setPosition(sf::Vector2f _pos) {
		m_position = _pos;
	}

	const sf::Vector2f getPosition() const {
		return {m_position};
	}

	sf::Vector2f m_position{0.f, 0.f};
};

#endif // ENTITY_H_INCLUDED
