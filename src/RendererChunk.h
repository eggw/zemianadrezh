#ifndef RENDERERCHUNK_H_INCLUDED
#define RENDERERCHUNK_H_INCLUDED

#include <map>

#include "Constants.h"

#include "RendererBase.h"
#include "World.h"
#include "TextureManager.h"

class RendererChunk : public RendererBase<WorldChunk> {
  public:
	RendererChunk(World& _world) : m_world{_world} {};

	~RendererChunk(){};

	//The publicly available update() function simply takes in a chunk's ID.
	//It then gets the pointer corresponding to that chunk, if it exists
	//in the world, and calls the private update() function on it.
	void update(int _chunkID) const {
		const WorldChunk* ptr{nullptr};

		for (auto& chunk : m_world.m_chunks) {
			if (chunk.getID() == _chunkID) {
				ptr = &chunk;
				break;
			}
		}

		if (ptr == nullptr) {
			return;
		}

		update(ptr);
	}

	void draw(sf::RenderTarget& target,
	          sf::RenderStates  states = sf::RenderStates::Default) const {
		static auto blocks{TextureManager::get_instance().getTexture(
		  TextureManager::Type::BLOCKS)};
		for (const auto& element : m_vertexMap) {
			target.draw(element.second, &blocks);
		}
	};

  private:
	//Functions -----------------------------------
	//When given a WorldChunk ptr, update() then generates a vertex array
	//for all the chunk's tiles. It then checks if the chunk is already present
	//in m_vertexMap. If so, it simply updates its vertex array, and if not, it
	//creates a new entry.
	void update(const WorldChunk* _chunk) const {
		sf::VertexArray vertexArray;
		vertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
		vertexArray.resize((Dimensions::Chunk::X * Dimensions::Chunk::Y) * 6);

		auto blocks = _chunk->getBlocks();

		int vertexIndex{0};

		for (auto& block : blocks) {
			sf::Vector2f topLeft{
			  block.getPixelPosition().x + _chunk->getPixelPosition().x,
			  block.getPixelPosition().y + _chunk->getPixelPosition().y};

			sf::Vector2f topRight{topLeft.x + Dimensions::Block::X, topLeft.y};

			sf::Vector2f bottomRight{topLeft.x + Dimensions::Block::X,
			                         topLeft.y + Dimensions::Block::Y};

			sf::Vector2f bottomLeft{topLeft.x,
			                        topLeft.y + Dimensions::Block::Y};

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
	//---------------------------------------------

	//Data members --------------------------------
	mutable std::map<const WorldChunk*, sf::VertexArray> m_vertexMap;
	const World&                                         m_world;
	//---------------------------------------------
};

#endif // RENDERERCHUNK_H_INCLUDED
