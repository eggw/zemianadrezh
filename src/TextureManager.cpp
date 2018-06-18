#include "TextureManager.h"

TextureManager::TextureManager(){
	sf::Texture player;

	player.loadFromFile("assets/gfx/player.png");
    m_textures.emplace(Type::PLAYER, player);
}

const sf::Texture& TextureManager::getTexture(Type _type){
	return m_textures[_type];
}
