#ifndef KEYBINDS_H_INCLUDED
#define KEYBINDS_H_INCLUDED

#include <SFML/Window/Keyboard.hpp>

//_A and _B denote that the two keys must be pressed together

namespace Key {
	const sf::Keyboard::Key 	JUMP_KEY 			= sf::Keyboard::Space;
	const sf::Keyboard::Key 	LEFT_KEY 			= sf::Keyboard::A;
	const sf::Keyboard::Key 	RIGHT_KEY 			= sf::Keyboard::D;

	const sf::Keyboard::Key 	CHAT_UP 			= sf::Keyboard::PageUp;
	const sf::Keyboard::Key 	CHAT_DOWN 			= sf::Keyboard::PageDown;
	const sf::Keyboard::Key 	CHAT_SEND   		= sf::Keyboard::Return;

	const sf::Keyboard::Key 	CHAT_TOP   			= sf::Keyboard::Home;
	const sf::Keyboard::Key 	CHAT_BOTTOM  		= sf::Keyboard::End;

	const sf::Keyboard::Key 	TEXT_SELECTALL_A 	= sf::Keyboard::LControl;
	const sf::Keyboard::Key 	TEXT_SELECTALL_B 	= sf::Keyboard::A;
	const sf::Keyboard::Key 	TEXT_SELECTLEFT_A   = sf::Keyboard::LControl;
	const sf::Keyboard::Key 	TEXT_SELECTLEFT_B   = sf::Keyboard::Left;
	const sf::Keyboard::Key 	TEXT_SELECTRIGHT_A  = sf::Keyboard::LControl;
	const sf::Keyboard::Key 	TEXT_SELECTRIGHT_B  = sf::Keyboard::Right;
	const sf::Keyboard::Key 	TEXT_MOVELEFT    	= sf::Keyboard::Left;
	const sf::Keyboard::Key 	TEXT_MOVERIGHT   	= sf::Keyboard::Right;
	const sf::Keyboard::Key 	TEXT_COPY_A         = sf::Keyboard::LControl;
	const sf::Keyboard::Key 	TEXT_COPY_B         = sf::Keyboard::C;
	const sf::Keyboard::Key 	TEXT_CUT_A          = sf::Keyboard::LControl;
	const sf::Keyboard::Key 	TEXT_CUT_B          = sf::Keyboard::X;
	const sf::Keyboard::Key 	TEXT_PASTE_A        = sf::Keyboard::LControl;
	const sf::Keyboard::Key 	TEXT_PASTE_B        = sf::Keyboard::V;

	const sf::Keyboard::Key 	PAUSE_KEY 			= sf::Keyboard::Escape;
}


#endif // KEYBINDS_H_INCLUDED
