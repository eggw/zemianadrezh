#ifndef COMPONENTSPLAYER_H_INCLUDED
#define COMPONENTSPLAYER_H_INCLUDED

#include "Components/ComponentAnimation.h"
#include "Components/ComponentDirection.h"
#include "Components/ComponentName.h"
#include "Components/ComponentPhysics.h"
#include "Components/ComponentPosition.h"

//This is simply a collection of all the Components a player
//needs, for syntax convenience in places like the network
//managers
struct ComponentsPlayer {
  public:
	ComponentsPlayer() : m_dir{}, m_name{}, m_vel{}, m_pos{} {};
	ComponentsPlayer(ComponentDirection _d,
	                 ComponentName      _n,
	                 ComponentPhysics   _v,
	                 ComponentPosition  _p)
	            : m_dir{_d}, m_name{_n}, m_vel{_v}, m_pos{_p} {};

	//Move assignment overload
	ComponentsPlayer& operator=(const ComponentsPlayer&) noexcept {
		return *this;
	};

	ComponentDirection m_dir{};
	ComponentName      m_name{};
	ComponentPhysics   m_vel{};
	ComponentPosition  m_pos{};
};

//Forward declare Packet here and define the overloading in .cpp, so
//we don't have to include SFML/Networking.hpp here
namespace sf {
	class Packet;
}
sf::Packet& operator<<(sf::Packet& _p, const ComponentsPlayer& _c);
sf::Packet& operator>>(sf::Packet& _p, ComponentsPlayer& _c);

#endif // COMPONENTSPRITE_H_INCLUDED
