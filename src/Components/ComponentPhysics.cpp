#include "ComponentPhysics.h"

#include <SFML/Network.hpp>

//Packet operator overloading
//----------------------------------------------------------------------------------------------------------------
sf::Packet& operator<<(sf::Packet& _p, const ComponentPhysics& _c) {
	return _p << _c.m_velocity.x << _c.m_velocity.y;
}

sf::Packet& operator>>(sf::Packet& _p, ComponentPhysics& _c) {
	return _p >> _c.m_velocity.x >> _c.m_velocity.y;
}
//----------------------------------------------------------------------------------------------------------------
