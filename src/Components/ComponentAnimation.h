#ifndef COMPONENTANIMATION_H_INCLUDED
#define COMPONENTANIMATION_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "AnimationMovement.h"

struct ComponentAnimation {
  public:
	ComponentAnimation(AnimationMovement _a)
	            : m_animation{_a} {};

	AnimationMovement m_animation;
};

#endif // COMPONENTSPRITE_H_INCLUDED
