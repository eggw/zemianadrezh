#include "Coordinates.h"

namespace Utility::Coordinates {
	int getIndex(sf::Vector2i _coords, int _rowLength) {
		return {(_coords.y * _rowLength) + _coords.x};
	}

	sf::Vector2i getCoords(int _index, int _rowLength) {
		return {_index % _rowLength, _index / _rowLength};
	}

	sf::Vector2i northOf(sf::Vector2i _coords) {
		return {_coords.x, _coords.y - 1};
	}
	sf::Vector2i southOf(sf::Vector2i _coords) {
		return {_coords.x, _coords.y + 1};
	}
	sf::Vector2i eastOf(sf::Vector2i _coords) {
		return {_coords.x + 1, _coords.y};
	}
	sf::Vector2i westOf(sf::Vector2i _coords) {
		return {_coords.x - 1, _coords.y};
	}

	bool outOfRange(sf::Vector2i _coords, const sf::Vector2i _size) {
		if (_coords.x < 0 || _coords.y < 0 || _coords.x >= _size.x ||
		    _coords.y >= _size.y) {
			return true;
		}
		return false;
	}

	bool outOfRange(int _index, const sf::Vector2i _size) {
		int minIndex{0};
		int maxIndex{(_size.x * _size.y) - 1};

		if (_index < minIndex || _index > maxIndex) {
			return true;
		}

		return false;
	}

} // namespace Utility::Coordinates
