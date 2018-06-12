#ifndef RENDERERCHUNK_H_INCLUDED
#define RENDERERCHUNK_H_INCLUDED

#include <map>

#include "SFML/Graphics.hpp"

#include "WorldChunk.h"

class RendererChunk {
  public:
  	RendererChunk(sf::RenderWindow& _window);
    void update(const WorldChunk* _chunk) const;
    void draw() const;

  private:
	sf::RenderWindow& m_window;
	mutable std::map<const WorldChunk*, sf::VertexArray> m_vertexMap;

};

#endif // RENDERERCHUNK_H_INCLUDED
