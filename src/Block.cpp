#include "Block.h"

Block::Block(bool _visibility)
	:m_visible(_visibility) {
}

bool Block::getVisibility() const {
	return m_visible;
}

void Block::setVisibility(bool _v) {
	m_visible = _v;
}
