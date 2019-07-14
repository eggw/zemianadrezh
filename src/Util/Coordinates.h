#ifndef COORDINATES_H_INCLUDED
#define COORDINATES_H_INCLUDED

#include <SFML/System/Vector2.hpp>

namespace Utility::Coordinates {

	int          getIndex(sf::Vector2i _coords, int _rowLength);
	sf::Vector2i getCoords(int _index, int _rowLength);

	sf::Vector2i northOf(sf::Vector2i _coords);
	sf::Vector2i southOf(sf::Vector2i _coords);
	sf::Vector2i eastOf(sf::Vector2i _coords);
	sf::Vector2i westOf(sf::Vector2i _coords);

	bool outOfRange(sf::Vector2i _coords, const sf::Vector2i _size);
	bool outOfRange(int _index, const sf::Vector2i _size);

} // namespace Utility::Coordinates

#endif // COORDINATES_H_INCLUDED
