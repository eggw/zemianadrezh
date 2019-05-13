//////////////////////////////////////////////|
//Contains tests at the end of the file       |
//////////////////////////////////////////////|
#include "debug.h"
//--------------------------------------------|
// (*) Blocks
//--------------------------------------------|
#include "Block.h"

#include "BlockDatabase.h"

Block::Block(int _id, BlockData::Type _type)
            : m_id{_id}, m_type{_type}, m_damageLevel{0} {
	initialisePosition();
}

BlockData::Type Block::getType() const {
	return m_type;
}

void Block::setType(BlockData::Type _t) {
	m_type = _t;
}

const BlockData& Block::getData() const {
	return BlockDatabase::get_instance().getData(m_type);
}

sf::FloatRect Block::getTextureRect() const {
	//First, fetch the coordinates for the first (0 damage level) texture of this block.
	sf::Vector2f textureCoordinates{
	  BlockDatabase::get_instance().getData(m_type).getTextureIndeces()};
	//We'll then make sure we multiply the indeces of the coordinates by the width and height
	//of a block, and add one pixel if they're greater than 0
	textureCoordinates.x *= BLOCK_DIMENSIONS_X;
	textureCoordinates.y *= BLOCK_DIMENSIONS_Y;

	if (textureCoordinates.x > 0) {
		++textureCoordinates.x;
	}
	if (textureCoordinates.y > 0) {
		++textureCoordinates.y;
	}

	//Since we have 7 damage levels for each block, we can divide the current damage level by
	//14 to get the X index for the texture we need.
	//For example, at a damage level of 53
	//53 / 14 = 3.78... (C++ floors int divisions) = 3

	//So we'll get the 4th (0-based indexing) damage level texture's coordinates, simply by
	//appending 3 * BLOCK_DIMENSIONS_X to textureCoordinates.x

	int damageIndex = m_damageLevel / 14;

	float newX{textureCoordinates.x + (damageIndex * BLOCK_DIMENSIONS_X)};

	return {newX,
	        textureCoordinates.y,
	        float(BLOCK_DIMENSIONS_X),
	        float(BLOCK_DIMENSIONS_Y)};
}

sf::Vector2f Block::getPosition() const {
	return m_position;
}

void Block::initialisePosition() {
	m_position.x = m_id % CHUNK_DIMENSIONS_X;
	m_position.y = m_id / CHUNK_DIMENSIONS_Y;
}

/////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------TESTS-------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////
TEST_CASE("Testing blocks") {
	Block b1{0, BlockData::Type::DIRT};

	SUBCASE("Testing block positioning") {
		CHECK(b1.getType() == BlockData::Type::DIRT);
		CHECK(b1.getPosition().x == 0);
		CHECK(b1.getPosition().y == 0);

		Block b2{1, BlockData::Type::DIRT};
		CHECK(b2.getPosition().x == 1);
		CHECK(b2.getPosition().y == 0);

		Block b3{31, BlockData::Type::DIRT};
		CHECK(b3.getPosition().x == 31);
		CHECK(b3.getPosition().y == 0);

		Block b4{32, BlockData::Type::DIRT};
		CHECK(b4.getPosition().x == 0);
		CHECK(b4.getPosition().y == 1);

		Block b5{1023, BlockData::Type::DIRT};
		CHECK(b5.getPosition().x == 31);
		CHECK(b5.getPosition().y == 31);
	}
}
/////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////
