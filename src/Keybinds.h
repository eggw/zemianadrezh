#ifndef KEYBINDS_H_INCLUDED
#define KEYBINDS_H_INCLUDED

#include <SFML/Window/Keyboard.hpp>

namespace Key {
	const sf::Keyboard::Key 	JUMP_KEY 		= sf::Keyboard::Space;
	const sf::Keyboard::Key 	LEFT_KEY 		= sf::Keyboard::A;
	const sf::Keyboard::Key 	RIGHT_KEY 		= sf::Keyboard::D;

	const sf::Keyboard::Key 	CHAT_UP 		= sf::Keyboard::PageUp;
	const sf::Keyboard::Key 	CHAT_DOWN 		= sf::Keyboard::PageDown;
	const sf::Keyboard::Key 	CHAT_SEND   	= sf::Keyboard::Return;

	const sf::Keyboard::Key 	CHAT_TOP   		= sf::Keyboard::Home;
	const sf::Keyboard::Key 	CHAT_BOTTOM  	= sf::Keyboard::End;

	const sf::Keyboard::Key 	PAUSE_KEY 		= sf::Keyboard::Escape;
}


#endif // KEYBINDS_H_INCLUDED
