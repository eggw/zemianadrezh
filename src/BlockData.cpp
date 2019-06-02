#include "BlockData.h"

#include <math.h>

BlockData::BlockData(std::string _name, int _opacity, int _textureColumn)
            : m_name{_name},
              m_opacityAlpha{int(ceil(2.55 * _opacity))},
              m_textureColumn{_textureColumn} {
}

const std::string& BlockData::getName() const {
	return m_name;
}

int BlockData::getOpacityAlpha() const {
	return m_opacityAlpha;
}

int BlockData::getTextureColumn() const {
	return m_textureColumn;
}
