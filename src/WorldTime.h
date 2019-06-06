#pragma once

#include <string>

#include "Util/HHMM.h"

class WorldTime {
  public:
	WorldTime(HHMM _t);
	void set(HHMM _t);
	//increment by one minute
	void tick();

	HHMM              get() const;
	const std::string getString() const;

	void pause() const;
	void unpause() const;

  private:
	//Data members --------------------------------
	HHMM         m_time;
	mutable bool m_paused;
	//---------------------------------------------
};

//Forward declare Packet here and define the overloading in .cpp, so
//we don't have to include SFML/Networking.hpp here
namespace sf {
	class Packet;
}
sf::Packet& operator<<(sf::Packet& _p, const HHMM& _t);
sf::Packet& operator>>(sf::Packet& _p, HHMM& _t);
