#include "RendererChunk.h"

RendererChunk::RendererChunk(sf::RenderWindow& _window)
    : m_window(_window) {
}

void RendererChunk::update(const WorldChunk* _chunk) const {
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

    auto it = m_vertexMap.find(_chunk);

    if(it != m_vertexMap.end()) {
        it->second = vertexArray;
    }
    else {
        m_vertexMap.emplace(_chunk, vertexArray);
    }
}

void RendererChunk::draw() const {
    for(auto& vertexArray : m_vertexMap) {
        m_window.draw(vertexArray.second);
    }
}
