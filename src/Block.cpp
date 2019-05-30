//////////////////////////////////////////////|
//Contains tests at the end of the file       |
//////////////////////////////////////////////|
#include "debug.h"
//--------------------------------------------|
// (*) Blocks
//--------------------------------------------|
#include "Block.h"

#include "BlockDatabase.h"

Block::Block(int _id, BlockData::Type _type, BlockData::BorderType _borderType)
            : m_id{_id}, m_type{_type}, m_borderType{_borderType}, m_damageLevel{0} {
	initialisePosition();
}

BlockData::Type Block::getType() const {
	return m_type;
}

void Block::setType(BlockData::Type _t) {
	m_type = _t;
}

void Block::setBorderType(BlockData::BorderType _t) {
	m_borderType = _t;
}

const BlockData& Block::getData() const {
	return BlockDatabase::get_instance().getData(m_type);
}

sf::FloatRect Block::getTextureRect() const {
	//First, fetch the coordinates for the first column in the row containing this block's
	//textures (the ALL BorderType)
	sf::Vector2f textureCoordinates{
	  BlockDatabase::get_instance().getData(m_type).getTextureIndeces()};
	//We'll then set the appropriate column depending on the BorderType
	textureCoordinates.x = m_borderType;
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

	return {textureCoordinates.x,
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
