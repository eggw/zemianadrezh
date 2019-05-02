#include "ComponentsPlayer.h"

#include <SFML/Network.hpp>

//Packet operator overloading
//----------------------------------------------------------------------------------------------------------------
sf::Packet& operator<<(sf::Packet& _p, const ComponentsPlayer& _c) {
	return _p << _c.compDir << _c.compName << _c.compVel << _c.compPos;
}

sf::Packet& operator>>(sf::Packet& _p, ComponentsPlayer& _c) {
	return _p >> _c.compDir >> _c.compName >> _c.compVel >> _c.compPos;
}
//----------------------------------------------------------------------------------------------------------------
