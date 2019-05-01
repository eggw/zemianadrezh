#include "Player.h"

#include <SFML/Network.hpp>

#include "TextureManager.h"
#include "Keybinds.h"
#include "InputLocker.h"

Player::Player(const std::string& _name) : m_playerName{_name} {
}

const std::string& Player::getName() const {
	return m_playerName;
}
