#ifndef BLOCKDATA_H_INCLUDED
#define BLOCKDATA_H_INCLUDED

#include <string>
#include <SFML/Graphics.hpp>

#include "TextureManager.h"

/*
BlockData contains all the information we need to know
about a certain block type.

m_textures is a map that contains the texture for this
block at every damage level. Since there are 7 textures,
there are 7 levels of damage. So it goes like this:

0-14 damage: first texture
14-28 damage: second texture

and so on. We get these values from TextureManager and insert them
in m_textures.
*/

class BlockData {
	public:
		enum class Type {
			AIR,
			DIRT
		};

		BlockData(	std::string _name,
					bool _visibile,
					TextureManager::Block _textureType);

		const std::string& getName() const;
		bool getVisible() const;
		sf::Texture& getTexture(int _damageLevel) const;
	private:
		std::string							m_name;
		bool 								m_visible;
		mutable std::map<int, sf::Texture>	m_textures;
};

#endif // BLOCKDATA_H_INCLUDED
