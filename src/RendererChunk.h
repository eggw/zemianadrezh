#ifndef RENDERERCHUNK_H_INCLUDED
#define RENDERERCHUNK_H_INCLUDED

#include <map>

#include "RendererBase.h"
#include "WorldChunk.h"
#include "TextureManager.h"

class RendererChunk : public RendererBase<WorldChunk> {
  public:
	RendererChunk(sf::RenderWindow& _window)
	            : RendererBase<WorldChunk>(_window){};

	~RendererChunk(){};

	void update(const WorldChunk* _chunk) {
		sf::VertexArray vertexArray;
		vertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
		vertexArray.resize((CHUNK_DIMENSIONS_X * CHUNK_DIMENSIONS_Y) * 6);

		auto blocks = _chunk->getBlocks();

		int vertexIndex{0};

		int xOffset = CHUNK_DIMENSIONS_X * BLOCK_DIMENSIONS_X * _chunk->getID();

		for (auto& block : blocks) {
			sf::Vector2f topLeft{block.getPosition().x * BLOCK_DIMENSIONS_X +
			                       xOffset,
			                     block.getPosition().y * BLOCK_DIMENSIONS_Y};

			sf::Vector2f topRight{topLeft.x + BLOCK_DIMENSIONS_X, topLeft.y};

			sf::Vector2f bottomRight{topLeft.x + BLOCK_DIMENSIONS_X,
			                         topLeft.y + BLOCK_DIMENSIONS_Y};

			sf::Vector2f bottomLeft{topLeft.x, topLeft.y + BLOCK_DIMENSIONS_Y};

			//-----------------------------------------------------
			sf::FloatRect textureRect{block.getTextureRect()};

			sf::Vector2f textureTopLeft{textureRect.left, textureRect.top};

			sf::Vector2f textureTopRight{textureRect.left + textureRect.width,
			                             textureRect.top};

			sf::Vector2f textureBottomRight{
			  textureRect.left + textureRect.width,
			  textureRect.top + textureRect.height};

			sf::Vector2f textureBottomLeft{
			  textureRect.left, textureRect.top + textureRect.height};
			//-----------------------------------------------------

			//top left
			vertexArray[vertexIndex].position = topLeft;
			vertexArray[vertexIndex].color.a =
			  block.getData().getOpacityAlpha();
			vertexArray[vertexIndex].texCoords = textureTopLeft;
			vertexIndex += 1;

			//top right
			vertexArray[vertexIndex].position = topRight;
			vertexArray[vertexIndex].color.a =
			  block.getData().getOpacityAlpha();
			vertexArray[vertexIndex].texCoords = textureTopRight;
			vertexIndex += 1;

			//bottom right
			vertexArray[vertexIndex].position = bottomRight;
			vertexArray[vertexIndex].color.a =
			  block.getData().getOpacityAlpha();
			vertexArray[vertexIndex].texCoords = textureBottomRight;
			vertexIndex += 1;

			//bottom right
			vertexArray[vertexIndex].position = bottomRight;
			vertexArray[vertexIndex].color.a =
			  block.getData().getOpacityAlpha();
			vertexArray[vertexIndex].texCoords = textureBottomRight;
			vertexIndex += 1;

			//bottom left
			vertexArray[vertexIndex].position = bottomLeft;
			vertexArray[vertexIndex].color.a =
			  block.getData().getOpacityAlpha();
			vertexArray[vertexIndex].texCoords = textureBottomLeft;
			vertexIndex += 1;

			//top left
			vertexArray[vertexIndex].position = topLeft;
			vertexArray[vertexIndex].color.a =
			  block.getData().getOpacityAlpha();
			vertexArray[vertexIndex].texCoords = textureTopLeft;
			vertexIndex += 1;
		}

		//If the chunk already exists in the map, we'll replace its
		//vertex array with the newly updated one.

		//Otherwise, we'll add a new key/value pair.

		bool found{false};

		for (auto& chunk : m_vertexMap) {
			if (chunk.first->getID() == _chunk->getID()) {
				chunk.second = vertexArray;
				found        = true;
				break;
			}
		}

		if (!found) {
			m_vertexMap.emplace(_chunk, vertexArray);
		}
	};

	void draw() const {
		static auto blocks{TextureManager::get_instance().getTexture(
		  TextureManager::Type::BLOCKS)};
		for (const auto& element : m_vertexMap) {
			RendererBase<WorldChunk>::m_window.draw(element.second, &blocks);
		}
	};

  private:
	mutable std::map<const WorldChunk*, sf::VertexArray> m_vertexMap;
};

#endif // RENDERERCHUNK_H_INCLUDED
