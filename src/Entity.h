#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <SFML/System/Vector2.hpp>

struct Entity {
	void setPosition(sf::Vector2f _pos){
		m_positionX = _pos.x;
		m_positionY = _pos.y;
	}

	const sf::Vector2f getPosition() const{
		return {m_positionX, m_positionY};
	}



    float m_positionX{0};
    float m_positionY{0};
};

#endif // ENTITY_H_INCLUDED
