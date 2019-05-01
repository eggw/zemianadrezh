#include "ComponentsPlayer.h"

#include <SFML/Network.hpp>

//Packet operator overloading
//----------------------------------------------------------------------------------------------------------------
sf::Packet& operator<<(sf::Packet& _p, const ComponentsPlayer& _c) {
	return _p << _c.m_dir << _c.m_name << _c.m_vel << _c.m_pos;
}

sf::Packet& operator>>(sf::Packet& _p, ComponentsPlayer& _c) {
	return _p >> _c.m_dir >> _c.m_name >> _c.m_vel >> _c.m_pos;
}
//----------------------------------------------------------------------------------------------------------------
