#ifndef TEXTUREMANAGER_H_INCLUDED
#define TEXTUREMANAGER_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "Singleton.h"

class TextureManager : public Singleton<TextureManager> {

	public:
		TextureManager();

		enum class Type {
			PLAYER
		};

		enum class Block {
			AIR,
			DIRT
		};

		const sf::Texture& getTexture(Type _type);
		const sf::Texture& getTexture(int _index, Block _block);

	private:
		std::map<Type, sf::Texture> m_textures;
		std::map<std::pair<int, Block>, sf::Texture> m_blockTextures;

};

#endif // TEXTUREMANAGER_H_INCLUDED
