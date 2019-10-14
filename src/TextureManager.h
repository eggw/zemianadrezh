#ifndef TEXTUREMANAGER_H_INCLUDED
#define TEXTUREMANAGER_H_INCLUDED

#include <SPSS/Util/Singleton.h>
#include <SPSS/System/ResourceManager.h>

#include <SFML/Graphics/Texture.hpp>

enum class TEXTURE
	{
		PLAYER,
		BLOCKS,
		SUN,
		MOON,
		CYCLE_SKY_GRADIENT,
		CYCLE_GLOW_GRADIENT
	};

class TextureManager : public spss::Singleton<TextureManager>,
                       public spss::ResourceManager<TEXTURE, sf::Texture> {
  public:
	inline TextureManager() {
		auto player{std::make_unique<sf::Texture>()};
		player->loadFromFile("assets/gfx/player.png");
		add(TEXTURE::PLAYER, std::move(player));

		auto blocks{std::make_unique<sf::Texture>()};
		blocks->loadFromFile("assets/gfx/blocks.png");
		add(TEXTURE::BLOCKS, std::move(blocks));

		auto sun{std::make_unique<sf::Texture>()};
		sun->loadFromFile("assets/gfx/sun.png");
		add(TEXTURE::SUN, std::move(sun));

		auto moon{std::make_unique<sf::Texture>()};
		moon->loadFromFile("assets/gfx/moon.png");
		add(TEXTURE::MOON, std::move(moon));

		auto sky{std::make_unique<sf::Texture>()};
		sky->loadFromFile("assets/shaders/sky.png");
		add(TEXTURE::CYCLE_SKY_GRADIENT, std::move(sky));

		auto glow{std::make_unique<sf::Texture>()};
		glow->loadFromFile("assets/shaders/glow.png");
		add(TEXTURE::CYCLE_GLOW_GRADIENT, std::move(glow));
	}
};

#endif // TEXTUREMANAGER_H_INCLUDED
