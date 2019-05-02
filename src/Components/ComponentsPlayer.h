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
	ComponentsPlayer() : compDir{}, compName{}, compVel{}, compPos{} {};
	ComponentsPlayer(ComponentDirection _d,
	                 ComponentName      _n,
	                 ComponentPhysics   _v,
	                 ComponentPosition  _p)
	            : compDir{_d}, compName{_n}, compVel{_v}, compPos{_p} {};

	//Move assignment overload
	ComponentsPlayer& operator=(const ComponentsPlayer&) noexcept {
		return *this;
	};

	ComponentDirection compDir{};
	ComponentName      compName{};
	ComponentPhysics   compVel{};
	ComponentPosition  compPos{};
};

//Forward declare Packet here and define the overloading in .cpp, so
//we don't have to include SFML/Networking.hpp here
namespace sf {
	class Packet;
}
sf::Packet& operator<<(sf::Packet& _p, const ComponentsPlayer& _c);
sf::Packet& operator>>(sf::Packet& _p, ComponentsPlayer& _c);

#endif // COMPONENTSPRITE_H_INCLUDED
