#ifndef KEYBINDS_H_INCLUDED
#define KEYBINDS_H_INCLUDED

#include <SFML/Window/Keyboard.hpp>

namespace Key {
	const sf::Keyboard::Key ZOOM_IN  = sf::Keyboard::Equal;
	const sf::Keyboard::Key ZOOM_OUT = sf::Keyboard::Hyphen;

	const sf::Keyboard::Key JUMP_KEY  = sf::Keyboard::Space;
	const sf::Keyboard::Key LEFT_KEY  = sf::Keyboard::A;
	const sf::Keyboard::Key RIGHT_KEY = sf::Keyboard::D;

	const sf::Keyboard::Key CHAT_UP   = sf::Keyboard::PageUp;
	const sf::Keyboard::Key CHAT_DOWN = sf::Keyboard::PageDown;
	const sf::Keyboard::Key CHAT_SEND = sf::Keyboard::Return;

	const sf::Keyboard::Key CHAT_TOP    = sf::Keyboard::Home;
	const sf::Keyboard::Key CHAT_BOTTOM = sf::Keyboard::End;

	const sf::Keyboard::Key PAUSE_KEY = sf::Keyboard::Escape;

	const sf::Keyboard::Key HOTBAR_1 = sf::Keyboard::Num1;
	const sf::Keyboard::Key HOTBAR_2 = sf::Keyboard::Num2;
	const sf::Keyboard::Key HOTBAR_3 = sf::Keyboard::Num3;
	const sf::Keyboard::Key HOTBAR_4 = sf::Keyboard::Num4;
	const sf::Keyboard::Key HOTBAR_5 = sf::Keyboard::Num5;
	const sf::Keyboard::Key HOTBAR_6 = sf::Keyboard::Num6;
	const sf::Keyboard::Key HOTBAR_7 = sf::Keyboard::Num7;
	const sf::Keyboard::Key HOTBAR_8 = sf::Keyboard::Num8;
	const sf::Keyboard::Key HOTBAR_9 = sf::Keyboard::Num9;
	const sf::Keyboard::Key HOTBAR_0 = sf::Keyboard::Num0;
} // namespace Key

#endif // KEYBINDS_H_INCLUDED
