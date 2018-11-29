#ifndef CHATBOXMESSAGE_H_INCLUDED
#define CHATBOXMESSAGE_H_INCLUDED

/*
TODO: add functions/make stuff private
*/

#include <SFML/Graphics.hpp>
#include "MulticolourText/MulticolourText.hpp"

class ChatBoxMessage {
	public:
		ChatBoxMessage(std::string _sender,
					   std::string _message,
					   const sf::Font& _font,
					   unsigned int _charSize,
					   unsigned int _m_numberOfLines = 1);

	public:
		sf::MulticolourText m_text;
		unsigned int m_senderLength;
		unsigned int m_numberOfLines;
};

#endif // CHATBOXMESSAGE_H_INCLUDED
