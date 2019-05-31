#include "Coordinates.h"

namespace Utility::Coordinates {
	int getIndex(sf::Vector2i _coords, int _rowLength) {
		 return {(_coords.y * _rowLength) + _coords.x};
	}

	sf::Vector2i getCoords(int _index, const sf::Vector2i _size) {
		return{_index % _size.x, _index / _size.y};
	}

	bool outOfRange(sf::Vector2i _coords, const sf::Vector2i _size) {
		if(_coords.x < 0 || _coords.y < 0 || _coords.x >= _size.x || _coords.y >= _size.y) {
			return true;
		}
		return false;
	}

	bool outOfRange(int _index, const sf::Vector2i _size) {
		int minIndex{0};
		int maxIndex{(_size.x * _size.y) - 1};

		if(_index < minIndex || _index > maxIndex) {
			return true;
		}

		return false;
	}

}
