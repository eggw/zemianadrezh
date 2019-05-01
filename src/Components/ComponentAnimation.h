#ifndef COMPONENTANIMATION_H_INCLUDED
#define COMPONENTANIMATION_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "Animation.h"

struct ComponentAnimation {
  public:
	ComponentAnimation(Animation _a) : m_animation{_a} {};

	//Move assignment overload
	ComponentAnimation& operator=(const ComponentAnimation&) noexcept {
		return *this;
	};

	Animation m_animation;
};

#endif // COMPONENTSPRITE_H_INCLUDED
