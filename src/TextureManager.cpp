#include "TextureManager.h"


constexpr int DIM_X = 16;
constexpr int DIM_Y = 16;

TextureManager::TextureManager() {
	sf::Texture player;
	player.loadFromFile("assets/gfx/player.png");
	m_textures.emplace(Type::PLAYER, player);

	for(int i{0}; i <=6; i++) {
        sf::Texture dirt;
        sf::IntRect dirtRect{i * DIM_X, 0, DIM_X, DIM_Y};
        dirt.loadFromFile("assets/gfx/blocks/dirt.png", dirtRect);
        m_blockTextures.emplace(std::make_pair(i,Block::DIRT), dirt);

        //temp: air has same texture as dirt but is just invisible
        m_blockTextures.emplace(std::make_pair(i,Block::AIR), dirt);
	}
}

const sf::Texture& TextureManager::getTexture(Type _type) {
	return m_textures[_type];
}

const sf::Texture& TextureManager::getTexture(int _index, Block _block) {
	return m_blockTextures[std::make_pair(_index,_block)];
}
