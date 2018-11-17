#ifndef ENTITYMOVING_H_INCLUDED
#define ENTITYMOVING_H_INCLUDED

#include "Entity.h"

struct EntityMoving : public Entity {
	sf::Vector2f m_velocity{0.f,0.f};
	bool m_facingLeft{false};
};

#endif // ENTITYMOVING_H_INCLUDED
