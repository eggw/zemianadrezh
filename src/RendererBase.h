#ifndef RENDERERBASE_H_INCLUDED
#define RENDERERBASE_H_INCLUDED

#include <map>

#include "SFML/Graphics.hpp"

template <class T>
class RendererBase {
	public:
		RendererBase(sf::RenderWindow& _window)
			:m_window(_window) {
		};

		virtual ~RendererBase() {
		};

		virtual void draw() const = 0;

	protected:
		sf::RenderWindow& m_window;
};

#endif // RENDERERBASE_H_INCLUDED
