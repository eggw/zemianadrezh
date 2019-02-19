#include "BlockDatabase.h"

#include "TextureManager.h"

BlockDatabase::BlockDatabase() :
		m_database{} {

	//temp: air has the same texture indeces as level 6 dirt, testing
	m_database.insert({BlockData::Type::AIR, {"air", true, {6,0}}});
	m_database.insert({BlockData::Type::DIRT, {"dirt", true, {0,0}}});

}

const BlockData& BlockDatabase::getData(BlockData::Type _t) {
	//todo: error checking?
	return m_database.at(_t);
}
