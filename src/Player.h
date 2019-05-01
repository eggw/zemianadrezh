#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Animation.h"

class Player {
  public:
	Player(const std::string& _name);
	const std::string& getName() const;

  private:
	std::string m_playerName;
	bool        m_spawned{false};
};
#endif // PLAYER_H_INCLUDED
