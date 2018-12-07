#ifndef ENTITYMOVING_H_INCLUDED
#define ENTITYMOVING_H_INCLUDED

#include "Entity.h"

struct EntityMoving : public Entity {
	virtual void update(int _timeslice) {
		m_position = {	m_position.x + m_velocity.x * _timeslice,
						m_position.y + m_velocity.y * _timeslice};
	}

	sf::Vector2f m_velocity{0.f,0.f};
	bool m_facingLeft{false};
};

#endif // ENTITYMOVING_H_INCLUDED
