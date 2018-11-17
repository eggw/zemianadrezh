#ifndef RENDERERCHUNK_H_INCLUDED
#define RENDERERCHUNK_H_INCLUDED

#include <map>

#include "RendererBase.h"
#include "WorldChunk.h"

#include <iostream>

class RendererChunk : public RendererBase<WorldChunk> {
	public:
		RendererChunk(sf::RenderWindow& _window)
			:RendererBase<WorldChunk>(_window) {
		};

		~RendererChunk() {
		};

		void update(const WorldChunk* _chunk) {
			sf::VertexArray vertexArray;
			vertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
			vertexArray.resize((CHUNK_DIMENSIONS_X * CHUNK_DIMENSIONS_Y) * 6);

			auto blocks = _chunk->getBlocks();

			int vertexIndex{0};

			int xOffset = CHUNK_DIMENSIONS_X * BLOCK_DIMENSIONS_X * _chunk->getID();

			for(auto& block : blocks) {
				if(!block.getVisibility()) {
					vertexIndex += 6;
					continue;
				}

				sf::Vector2f topLeft{   block.m_position.x * BLOCK_DIMENSIONS_X + xOffset,
										block.m_position.y * BLOCK_DIMENSIONS_Y};

				sf::Vector2f topRight{  topLeft.x + BLOCK_DIMENSIONS_X,
										topLeft.y};

				sf::Vector2f bottomRight{topLeft.x + BLOCK_DIMENSIONS_X,
										 topLeft.y + BLOCK_DIMENSIONS_Y};

				sf::Vector2f bottomLeft{topLeft.x,
										topLeft.y + BLOCK_DIMENSIONS_Y};

				//top left
				vertexArray[vertexIndex].position = topLeft;
				vertexArray[vertexIndex].color = sf::Color::Green;
				vertexIndex += 1;

				//top right
				vertexArray[vertexIndex].position = topRight;
				vertexArray[vertexIndex].color = sf::Color::Red;
				vertexIndex += 1;

				//bottom right
				vertexArray[vertexIndex].position = bottomRight;
				vertexArray[vertexIndex].color = sf::Color::Blue;
				vertexIndex += 1;

				//bottom right
				vertexArray[vertexIndex].position = bottomRight;
				vertexArray[vertexIndex].color = sf::Color::Blue;
				vertexIndex += 1;

				//bottom left
				vertexArray[vertexIndex].position = bottomLeft;
				vertexArray[vertexIndex].color = sf::Color::Yellow;
				vertexIndex += 1;

				//top left
				vertexArray[vertexIndex].position = topLeft;
				vertexArray[vertexIndex].color = sf::Color::Green;

				vertexIndex += 1;
			}

			//If the chunk already exists in the map, we'll replace its
			//vertex array with the newly updated one.

			//Otherwise, we'll add a new key/value pair.

			bool found{false};

			for(auto& chunk : m_vertexMap) {
				if(chunk.first->getID() == _chunk->getID()) {
					chunk.second = vertexArray;
					found = true;
					break;
				}
			}

			if(!found) {
				m_vertexMap.emplace(_chunk, vertexArray);
			}
		};

		void draw() const {
			for(auto& vertexArray : m_vertexMap) {
				RendererBase<WorldChunk>::m_window.draw(vertexArray.second);
			}
		};

	private:
		mutable std::map<const WorldChunk*, sf::VertexArray> m_vertexMap;
};

#endif // RENDERERCHUNK_H_INCLUDED
