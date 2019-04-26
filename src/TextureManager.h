#ifndef TEXTUREMANAGER_H_INCLUDED
#define TEXTUREMANAGER_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "Singleton.h"

class TextureManager : public Singleton<TextureManager> {

	public:
		TextureManager();

		enum class Type {
			PLAYER,
			BLOCKS,
			SUN,
			MOON,
			CYCLE_SKY_GRADIENT,
			CYCLE_GLOW_GRADIENT
		};

		const sf::Texture& getTexture(Type _type);

	private:
		std::map<Type, sf::Texture> m_textures;

};

#endif // TEXTUREMANAGER_H_INCLUDED
