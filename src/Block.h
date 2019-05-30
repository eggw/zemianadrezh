#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include "BlockData.h"

/*
Blocks are initialised by specifying a type from BlockData::Type.
Information (such as the texture) about the block is obtained via
a lookup in the singleton BlockDatabase.

BlockDatabase has a map of Types and BlockData. So, for example, if
we have a grass block and we want to get the texture to draw it, we
simply call BlockDatabase::getData() and pass the block's type as the
argument. This will return a constant reference to the BlockData object.

This way, each block only needs to have an index, specifying its position
in its chunk (m_id), a block type (m_type), and its damage level (m_damageLevel).

Note that blocks aren't defined as entities.
*/

class Block {
  public:
	Block(int _id, BlockData::Type _type, BlockData::BorderType _borderType = BlockData::BorderType::ALL);

	BlockData::Type  getType() const;
	void             setType(BlockData::Type _t);
	void             setBorderType(BlockData::BorderType _t);
	const BlockData& getData() const;
	sf::FloatRect    getTextureRect() const;
	sf::Vector2f     getPosition() const;

  private:
	//Functions -----------------------------------
	//Based on the id, this function will set the
	//appropriate position in a 2D coordinate
	//system.
	void initialisePosition();
	//---------------------------------------------

	//Data members --------------------------------
	int                   m_id;
	BlockData::Type       m_type;
	BlockData::BorderType m_borderType;
	int                   m_damageLevel;
	sf::Vector2f          m_position;
	//---------------------------------------------
};

#endif // BLOCK_H_INCLUDED
