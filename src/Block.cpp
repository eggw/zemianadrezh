#include "Block.h"

#include "BlockDatabase.h"

Block::Block(int _id,
			BlockData::Type _type) :
	m_id{_id},
	m_type {_type},
	m_damageLevel {0}
{

}

void Block::setType(BlockData::Type _t) {
	m_type = _t;
}

const BlockData& Block::getData() const {
	return BlockDatabase::get_instance().getData(m_type);
}

sf::Texture& Block::getTexture() const {
	return BlockDatabase::get_instance().getData(m_type).getTexture(m_damageLevel);
}
