#include "BlockData.h"

#include <math.h>

BlockData::BlockData(std::string  _name,
                     int          _opacity,
                     sf::Vector2f _textureIndeces)
            : m_name{_name},
              m_opacityAlpha{int(ceil(2.55 * _opacity))},
              m_textureIndeces{_textureIndeces} {
}

const std::string& BlockData::getName() const {
	return m_name;
}

int BlockData::getOpacityAlpha() const {
	return m_opacityAlpha;
}

sf::Vector2f BlockData::getTextureIndeces() const {
	return m_textureIndeces;
}
