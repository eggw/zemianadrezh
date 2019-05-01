#include "ComponentDirection.h"

#include <SFML/Network.hpp>

//Packet operator overloading
//----------------------------------------------------------------------------------------------------------------
sf::Packet& operator<<(sf::Packet& _p, const ComponentDirection& _c) {
	return _p << _c.m_facingLeft;
}

sf::Packet& operator>>(sf::Packet& _p, ComponentDirection& _c) {
	return _p >> _c.m_facingLeft;
}
//----------------------------------------------------------------------------------------------------------------
