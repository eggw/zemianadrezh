#include "ComponentPosition.h"

#include <SFML/Network.hpp>

//Packet operator overloading
//----------------------------------------------------------------------------------------------------------------
sf::Packet& operator<<(sf::Packet& _p, const ComponentPosition& _c) {
	return _p << _c.m_position.x << _c.m_position.y;
}

sf::Packet& operator>>(sf::Packet& _p, ComponentPosition& _c) {
	return _p >> _c.m_position.x >> _c.m_position.y;
}
//----------------------------------------------------------------------------------------------------------------
