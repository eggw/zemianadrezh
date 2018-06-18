#ifndef ENTITYMOVING_H_INCLUDED
#define ENTITYMOVING_H_INCLUDED

#include "Entity.h"

struct EntityMoving : public Entity{
    float m_speed{1.0f};
    bool m_facingLeft{false};
};

#endif // ENTITYMOVING_H_INCLUDED
