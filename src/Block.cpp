//////////////////////////////////////////////|
//Contains tests at the end of the file       |
//////////////////////////////////////////////|
#include "debug.h"
//--------------------------------------------|
// (*) Blocks
//--------------------------------------------|
#include "Block.h"
#include "BlockDatabase.h"
#include "Util/Coordinates.h"

Block::Block(int _id, BlockData::Type _type, BlockData::BorderType _borderType)
            : m_id{_id}, m_type{_type}, m_borderType{_borderType}, m_damageLevel{0} {
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
	//Get the appropriate row depending on the BorderType,
	//and the column of the texture of the block type, and
	//multiply the indeces of the coordinates by the the
	//dimensions of a block.

	sf::Vector2f textureCoordinates{
	  float(m_borderType * BLOCK_DIMENSIONS_X),
	  float(BlockDatabase::get_instance().getData(m_type).getTextureColumn() * BLOCK_DIMENSIONS_Y)};

	return {textureCoordinates.x,
	        textureCoordinates.y,
	        float(BLOCK_DIMENSIONS_X),
	        float(BLOCK_DIMENSIONS_Y)};
}

sf::Vector2i Block::getPosition() const {
	return {Utility::Coordinates::getCoords(m_id, {CHUNK_DIMENSIONS_X,
												   CHUNK_DIMENSIONS_Y})};
}

//This function determines each block's border type.
//It does so by ruling out, one by one, the border
//types the block CANNOT have, depending on its
//neighbors, until it is left with one final
//result.

void Block::adjustBorders(NeighboringBlocks& _neighbors) {
	//The 16 types correspond to
	//BlockData::BorderType
	static std::vector<int> possibleBorders{
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
	};

	auto borders{possibleBorders};

	if(_neighbors[Direction::NORTH] != nullptr) {
		borders.erase(std::remove_if(
		borders.begin(), borders.end(),
		[](const int& x) {
			return x == 10
			|| (x >= 0 && x <= 3)
			|| (x >= 13 && x <= 15);
		}), borders.end());
	}
	else {
		borders.erase(std::remove_if(
		borders.begin(), borders.end(),
		[](const int& x) {
			return x == 11
			|| x == 12
			|| (x >= 4 && x <= 9);
		}), borders.end());
	}

	if(_neighbors[Direction::SOUTH] != nullptr) {
		borders.erase(std::remove_if(
		borders.begin(), borders.end(),
		[](const int& x) {
			return x == 0
			|| x == 12
			|| x == 14
			|| x == 15
			|| (x >= 7 && x <= 10);
		}), borders.end());
	}
	else {
		borders.erase(std::remove_if(
		borders.begin(), borders.end(),
		[](const int& x) {
			return x == 11
			|| x == 13
			|| (x >= 1 && x <= 6);
		}), borders.end());
	}

	if(_neighbors[Direction::EAST] != nullptr) {
		borders.erase(std::remove_if(
		borders.begin(), borders.end(),
		[](const int& x) {
			return x == 0
			|| x == 3
			|| x == 6
			|| x == 9
			|| x == 15
			|| (x >= 11 && x <= 13);
		}), borders.end());
	}
	else {
		borders.erase(std::remove_if(
		borders.begin(), borders.end(),
		[](const int& x) {
			return x == 1
			|| x == 2
			|| x == 4
			|| x == 5
			|| x == 7
			|| x == 8
			|| x == 10
			|| x == 14;
		}), borders.end());
	}

	if(_neighbors[Direction::WEST] != nullptr) {
		borders.erase(std::remove_if(
		borders.begin(), borders.end(),
		[](const int& x) {
			return x == 0
			|| x == 1
			|| x == 4
			|| x == 7
			|| (x >= 11 && x <= 14);
		}), borders.end());
	}
	else {
		borders.erase(std::remove_if(
		borders.begin(), borders.end(),
		[](const int& x) {
			return x == 2
			|| x == 3
			|| x == 5
			|| x == 6
			|| x == 15
			|| (x >= 8 && x <= 10);
		}), borders.end());
	}

	//By now, all but 1 border types should have been removed
	//from the borders vector.
	m_borderType = BlockData::BorderType(borders[0]);
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
