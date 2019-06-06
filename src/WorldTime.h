#pragma once

#include <string>

struct HHMM {
	HHMM(int _h, int _m) : hours{_h}, minutes{_m} {};
	HHMM(const HHMM& _t) : hours{_t.hours}, minutes{_t.minutes} {};
	int hours;
	int minutes;
};

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
