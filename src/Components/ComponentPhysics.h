#ifndef COMPONENTVELOCITY_H_INCLUDED
#define COMPONENTVELOCITY_H_INCLUDED

#include <SFML/System/Vector2.hpp>

struct ComponentPhysics {
  public:
	ComponentPhysics() : m_velocity{0, 0} {};
	ComponentPhysics(sf::Vector2f _v) : m_velocity{_v} {};
	ComponentPhysics(float _x, float _y) : m_velocity{_x, _y} {};

	//Move assignment overload
	ComponentPhysics& operator=(const ComponentPhysics&) noexcept {
		return *this;
	};

	sf::Vector2f m_velocity;
};

//Forward declare Packet here and define the overloading in .cpp, so
//we don't have to include SFML/Networking.hpp here
namespace sf {
	class Packet;
}
sf::Packet& operator<<(sf::Packet& _p, const ComponentPhysics& _c);
sf::Packet& operator>>(sf::Packet& _p, ComponentPhysics& _c);

#endif // COMPONENTVELOCITY_H_INCLUDED
