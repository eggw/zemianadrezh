#ifndef TEXTUREMANAGER_H_INCLUDED
#define TEXTUREMANAGER_H_INCLUDED

#include <SFML/Graphics.hpp>

#include <SPSS/Util/Singleton.h>

class TextureManager : public spss::Singleton<TextureManager> {
  public:
	TextureManager();

	enum class TYPE
	{
		PLAYER,
		BLOCKS,
		SUN,
		MOON,
		CYCLE_SKY_GRADIENT,
		CYCLE_GLOW_GRADIENT
	};

	const sf::Texture& getTexture(TextureManager::TYPE _type);

  private:
	//Data members --------------------------------
	std::map<TextureManager::TYPE, sf::Texture> m_textures;
	//---------------------------------------------
};

#endif // TEXTUREMANAGER_H_INCLUDED
