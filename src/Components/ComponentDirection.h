#ifndef COMPONENTDIRECTION_H_INCLUDED
#define COMPONENTDIRECTION_H_INCLUDED

#include <SFML/Graphics.hpp>

struct ComponentDirection {
  public:
	ComponentDirection() : m_facingLeft{true} {};
	ComponentDirection(bool _left) : m_facingLeft{_left} {};

	bool m_facingLeft;
};

//Forward declare Packet here and define the overloading in .cpp, so
//we don't have to include SFML/Networking.hpp here
namespace sf {
	class Packet;
}
sf::Packet& operator<<(sf::Packet& _p, const ComponentDirection& _c);
sf::Packet& operator>>(sf::Packet& _p, ComponentDirection& _c);

#endif // COMPONENTSPRITE_H_INCLUDED
