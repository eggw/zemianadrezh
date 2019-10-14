#ifndef BLOCKDATABASE_H_INCLUDED
#define BLOCKDATABASE_H_INCLUDED

#include <map>

#include <SPSS/Util/Singleton.h>
#include <SPSS/System/ResourceManager.h>

#include "BlockData.h"

/*
BlockDatabase is a singleton whose function is to fetch the BlockData
of a given BlockData::Type.

This way, we don't have to redundantly store countless copies of BlockData
in every block.
*/

class BlockDatabase : public spss::Singleton<BlockDatabase>,
                      public spss::ResourceManager<BlockData::Type, BlockData> {
  public:
	inline BlockDatabase() {
		//Since air has an opacity of 0, it's not really worth creating
		//an exception for it in the code logic to not take in texture
		//indeces. Instead, we can just give it any index because it
		//doesn't matter.

		auto air{std::make_unique<BlockData>("air", 0, 0)};
		auto dirt{std::make_unique<BlockData>("dirt", 100, 0)};
		auto grass{std::make_unique<BlockData>("grass", 100, 1)};

		add(BlockData::Type::AIR, std::move(air));
		add(BlockData::Type::DIRT, std::move(dirt));
		add(BlockData::Type::GRASS, std::move(grass));
	}
};

#endif // BLOCKDATABASE_H_INCLUDED
