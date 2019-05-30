#include "Coordinates.h"

namespace Utility::Coordinates {
	int coordinatesToIndex(sf::Vector2i _c, int _rowLength) {
		 return {(_c.y * _rowLength) + _c.x};
	}

	sf::Vector2i indexToCoordinates(int _index, const sf::Vector2i _size) {
		return{_index % _size.x, _index / _size.y};
	}

	bool coordinatesOutOfRange(sf::Vector2i _c, const sf::Vector2i _size) {
		if(_c.x < 0 || _c.y < 0 || _c.x >= _size.x || _c.y >= _size.y) {
			return true;
		}
		return false;
	}

	bool indexOutOfRange(int _index, const sf::Vector2i _size) {
		int minIndex{0};
		int maxIndex{(_size.x * _size.y) - 1};

		if(_index < minIndex || _index > maxIndex) {
			return true;
		}

		return false;
	}

}
