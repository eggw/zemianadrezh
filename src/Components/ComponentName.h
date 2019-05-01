#ifndef COMPONENTNAME_H_INCLUDED
#define COMPONENTNAME_H_INCLUDED

#include <string>

struct ComponentName {
  public:
	ComponentName() : m_name{"[BLANK]"} {};
	ComponentName(std::string _str) : m_name{_str} {};

	//Move assignment overload
	ComponentName& operator=(const ComponentName&) noexcept {
		return *this;
	};

	std::string m_name;
};

//Forward declare Packet here and define the overloading in .cpp, so
//we don't have to include SFML/Networking.hpp here
namespace sf {
	class Packet;
}
sf::Packet& operator<<(sf::Packet& _p, const ComponentName& _c);
sf::Packet& operator>>(sf::Packet& _p, ComponentName& _c);

#endif // COMPONENTSPRITE_H_INCLUDED
