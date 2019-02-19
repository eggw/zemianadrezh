#include "TextureManager.h"

TextureManager::TextureManager() {
	sf::Texture player;
	player.loadFromFile("assets/gfx/player.png");
	m_textures.emplace(Type::PLAYER, player);

	sf::Texture blocks;
	blocks.loadFromFile("assets/gfx/blocks.png");
	m_textures.emplace(Type::BLOCKS, blocks);
}

const sf::Texture& TextureManager::getTexture(Type _type) {
	return m_textures[_type];
}
