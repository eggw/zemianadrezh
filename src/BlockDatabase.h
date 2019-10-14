#ifndef BLOCKDATABASE_H_INCLUDED
#define BLOCKDATABASE_H_INCLUDED

#include <map>

#include <SPSS/Util/Singleton.h>
#include "BlockData.h"

/*
BlockDatabase is a singleton whose function is to fetch the BlockData
of a given BlockData::Type.

This way, we don't have to redundantly store countless copies of BlockData
in every block.
*/

class BlockDatabase : public spss::Singleton<BlockDatabase> {
  public:
	BlockDatabase();
	const BlockData& getData(BlockData::Type _t);

  private:
	//Data members --------------------------------
	std::map<BlockData::Type, BlockData> m_database;
	//---------------------------------------------
};

#endif // BLOCKDATABASE_H_INCLUDED
