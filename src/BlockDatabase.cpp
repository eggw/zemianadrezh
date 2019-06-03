#include "BlockDatabase.h"

#include "TextureManager.h"

BlockDatabase::BlockDatabase() : m_database{} {
	//Since air has an opacity of 0, it's not really worth creating
	//an exception for it in the code logic to not take in texture
	//indeces. Instead, we can just give it any index because it
	//doesn't matter.
	m_database.insert({BlockData::Type::AIR, {"air", 0, 0}});
	m_database.insert({BlockData::Type::DIRT, {"dirt", 100, 0}});
	m_database.insert({BlockData::Type::GRASS, {"grass", 100, 1}});
}

const BlockData& BlockDatabase::getData(BlockData::Type _t) {
	//todo: error checking?
	return m_database.at(_t);
}
