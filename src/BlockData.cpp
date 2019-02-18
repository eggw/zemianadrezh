#include "BlockData.h"

BlockData::BlockData(std::string _name,
					bool _visible,
					TextureManager::Block _textureType) :
	m_name{_name},
	m_visible{_visible},
	m_textures{}
{
	//Every 14 steps (0,14,28,42,etc.) we'll insert a key-pair value
	//containing that number and the texture for the block at that
	//level of damage
	for(int i{0}; i <= 6; i++) {
        auto texture{TextureManager::get_instance().getTexture(i, _textureType)};
        m_textures.insert(std::make_pair(i*14,texture));
	}
}

const std::string& BlockData::getName() const {
	return m_name;
}

bool BlockData::getVisible() const {
	return m_visible;
}

//If passed a damage level of, say, 53, we'll use the appropriate
//texture (the one between 42 and 56; i.e., the entry in m_textures
//with a key of 42)
sf::Texture& BlockData::getTexture(int _damageLevel) const {
    int mapKey = _damageLevel - (_damageLevel % 14);
    return m_textures[mapKey];
}
