#ifndef RENDERERBASE_H_INCLUDED
#define RENDERERBASE_H_INCLUDED

#include <map>

#include "SFML/Graphics.hpp"

template<class T>
class RendererBase {
  public:
	RendererBase(){};

	virtual ~RendererBase(){};

	virtual void draw(
	  sf::RenderTarget& target,
	  sf::RenderStates  states = sf::RenderStates::Default) const = 0;
};

#endif // RENDERERBASE_H_INCLUDED
