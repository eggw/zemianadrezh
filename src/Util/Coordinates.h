#ifndef COORDINATES_H_INCLUDED
#define COORDINATES_H_INCLUDED

#include <SFML/System/Vector2.hpp>


namespace Utility::Coordinates {

	int          coordinatesToIndex(sf::Vector2i _c, int _rowLength);
	sf::Vector2i indexToCoordinates(int _index, const sf::Vector2i _size);

	bool         coordinatesOutOfRange(sf::Vector2i _c, const sf::Vector2i _size);
	bool         indexOutOfRange(int _index, const sf::Vector2i _size);

}

#endif // COORDINATES_H_INCLUDED
