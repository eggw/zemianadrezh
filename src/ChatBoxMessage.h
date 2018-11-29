#ifndef CHATBOXMESSAGE_H_INCLUDED
#define CHATBOXMESSAGE_H_INCLUDED

/*
TODO: add functions/make stuff private
*/

#include <SFML/Graphics.hpp>
#include "MulticolourText/MulticolourText.hpp"

class ChatBoxMessage : public sf::Drawable {
	public:
		ChatBoxMessage(std::string _sender,
					   std::string _message,
					   const sf::Font& _font,
					   unsigned int _charSize);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        void fitWidth(float _width);

        void setString(const std::string& _str);
        void setPosition(sf::Vector2f _pos);
        void setTransparency(int _a);

        sf::Vector2f getPosition() const;
        int getTransparency() const;
        unsigned int getNumberOfLines() const;

    private:
		sf::MulticolourText m_text;
		std::string m_sender;
		std::string m_message;
};

#endif // CHATBOXMESSAGE_H_INCLUDED
