#include "BlockDatabase.h"

#include "TextureManager.h"

BlockDatabase::BlockDatabase() :
		m_database{} {

	m_database.insert({BlockData::Type::AIR, {"air", false, TextureManager::Block::AIR}});
	m_database.insert({BlockData::Type::DIRT, {"dirt", true, TextureManager::Block::DIRT}});
}

const BlockData& BlockDatabase::getData(BlockData::Type _t) {
	//todo: error checking?
	return m_database.at(_t);
}
