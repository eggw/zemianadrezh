#include "BlockData.h"

BlockData::BlockData(std::string _name,
					bool _visible,
					sf::Vector2f _textureIndeces) :
	m_name{_name},
	m_visible{_visible},
	m_textureIndeces{_textureIndeces}
{
}

const std::string& BlockData::getName() const {
	return m_name;
}

bool BlockData::getVisible() const {
	return m_visible;
}

sf::Vector2f BlockData::getTextureIndeces() const {
	return m_textureIndeces;
}
