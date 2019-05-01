#ifndef COMPONENTPOSITION_H_INCLUDED
#define COMPONENTPOSITION_H_INCLUDED

#include <SFML/System/Vector2.hpp>

struct ComponentPosition {
  public:
	ComponentPosition() : m_position{0, 0} {};
	ComponentPosition(sf::Vector2f _v) : m_position{_v} {};
	ComponentPosition(float _x, float _y) : m_position{_x, _y} {};

	//Move assignment overload
	ComponentPosition& operator=(const ComponentPosition&) noexcept {
		return *this;
	};

	sf::Vector2f m_position;
};

//Forward declare Packet here and define the overloading in .cpp, so
//we don't have to include SFML/Networking.hpp here
namespace sf {
	class Packet;
}
sf::Packet& operator<<(sf::Packet& _p, const ComponentPosition& _c);
sf::Packet& operator>>(sf::Packet& _p, ComponentPosition& _c);

#endif // COMPONENTPOSITION_H_INCLUDED
