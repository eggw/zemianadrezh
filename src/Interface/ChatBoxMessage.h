#ifndef CHATBOXMESSAGE_H_INCLUDED
#define CHATBOXMESSAGE_H_INCLUDED

/*
TODO: add functions/make stuff private
*/

#include <SPSS/Graphics/MulticolorText.h>
#include <SFML/Graphics.hpp>

#include "Message.h"

class ChatBoxMessage : public sf::Drawable {
  public:
	ChatBoxMessage(const Message&  _msg,
	               const sf::Font& _font,
	               unsigned int    _charSize);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void fitWidth(float _width);

	void setString(const std::string& _str);
	void setPosition(sf::Vector2f _pos);
	void setTransparency(int _a);

	sf::Vector2f getPosition() const;
	int          getTransparency() const;
	unsigned int getNumberOfLines(size_t _startPos = 0,
	                              size_t _endPos   = 0) const;

  private:
	//Data members --------------------------------
	spss::MulticolorText m_text;
	Message               m_message;
	//---------------------------------------------
};

#endif // CHATBOXMESSAGE_H_INCLUDED
