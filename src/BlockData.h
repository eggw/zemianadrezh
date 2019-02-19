#ifndef BLOCKDATA_H_INCLUDED
#define BLOCKDATA_H_INCLUDED

#include <string>
#include <SFML/Graphics.hpp>

constexpr int BLOCK_DIMENSIONS_X = 16;
constexpr int BLOCK_DIMENSIONS_Y = 16;

/*
BlockData contains all the information we need to know
about a certain block type.

assets/gfx/blocks.png is a texture atlas that contains all the
block textures. Every row will contain 7 16x16 textures, one for
each damage level of the block.

m_textureIndeces describes the coordinates for the first, or damage
level 0, texture in the atlas. For example, our very first block would
have an index of (0,0). If our second block was on the next row, its index
would be (1,0). The indeces are automatically multiplied by the dimensions
of the block.
*/

class BlockData {
	public:
		enum Type {
			AIR = 0,
			DIRT = 1
		};

		BlockData(	std::string _name,
					bool _visibile,
					sf::Vector2f _textureIndeces);

		const std::string& getName() const;
		bool getVisible() const;
		sf::Vector2f getTextureIndeces() const;
	private:
		std::string		m_name;
		bool 			m_visible;
		sf::Vector2f 	m_textureIndeces;
};

#endif // BLOCKDATA_H_INCLUDED
