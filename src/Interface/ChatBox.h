#ifndef CHATBOX_H_INCLUDED
#define CHATBOX_H_INCLUDED

#include <SPSS/Graphics/InfoBox.h>
#include <SPSS/Graphics/TextEntryBox.h>
#include <SPSS/Util/Message.h>

#include <SFML/Graphics.hpp>

#include "Interface/ChatBoxMessage.h"

class ChatBox : public sf::Drawable {
  public:
	ChatBox(const std::string& _name);

	void appendMessage(const spss::Message& _msg);
	void getInput(sf::Event& _event);
	void update();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	bool completedMessage(spss::Message* _ptr = nullptr);

  private:
  	//Functions -----------------------------------
	void  onResize(sf::Vector2u _newSize);

	//Data members --------------------------------
	mutable sf::RenderTarget* m_target;
	sf::Vector2u              m_lastTargetSize;

	const std::string m_name;

	spss::InfoBox      m_infoBox;
	sf::View           m_view;

	spss::Message               m_lastMessage;
	spss::TextEntryBox          m_textEntry;
	//m_clock will be used to determine when the
	//messages in the chatbox will begin to fade
	//away.
	sf::Clock m_clock;
	//---------------------------------------------
};

#endif // CHATBOX_H_INCLUDED
