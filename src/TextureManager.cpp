#include "TextureManager.h"

TextureManager::TextureManager() {
	sf::Texture player;
	player.loadFromFile("assets/gfx/player.png");
	m_textures.emplace(Type::PLAYER, player);

	sf::Texture blocks;
	blocks.loadFromFile("assets/gfx/blocks.png");
	m_textures.emplace(Type::BLOCKS, blocks);

	sf::Texture sun;
	sun.loadFromFile("assets/gfx/sun.png");
	m_textures.emplace(Type::SUN, sun);

	sf::Texture moon;
	moon.loadFromFile("assets/gfx/moon.png");
	m_textures.emplace(Type::MOON, moon);

	sf::Texture sky;
	sky.loadFromFile("assets/shaders/sky.png");
	m_textures.emplace(Type::CYCLE_SKY_GRADIENT, sky);

	sf::Texture glow;
	glow.loadFromFile("assets/shaders/glow.png");
	m_textures.emplace(Type::CYCLE_GLOW_GRADIENT, glow);
}

const sf::Texture& TextureManager::getTexture(Type _type) {
	return m_textures[_type];
}
