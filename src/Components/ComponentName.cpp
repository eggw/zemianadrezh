#include "ComponentName.h"

#include <SFML/Network.hpp>

//Packet operator overloading
//----------------------------------------------------------------------------------------------------------------
sf::Packet& operator<<(sf::Packet& _p, const ComponentName& _c) {
	return _p << _c.m_name;
}

sf::Packet& operator>>(sf::Packet& _p, ComponentName& _c) {
	return _p >> _c.m_name;
}
//----------------------------------------------------------------------------------------------------------------
