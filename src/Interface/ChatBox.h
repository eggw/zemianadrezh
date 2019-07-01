#ifndef CHATBOX_H_INCLUDED
#define CHATBOX_H_INCLUDED

#include <SPSS/Graphics/TextEntryBox.h>
#include <SPSS/Graphics/MulticolorText.h>
#include <SFML/Graphics.hpp>

#include "Interface/ChatBoxMessage.h"
#include "Message.h"

class ChatBox : public sf::Drawable {
  public:
	ChatBox(const std::string& _name);

	void appendMessage(const Message _msg);
	void getInput(sf::Event& _event);
	void update();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	bool completedMessage(Message* _ptr = nullptr);

  private:
	//Functions -----------------------------------
	//
	//Message-related
	void positionMessage(int _index);
	void setTransparency(int _a);
	bool messagesTransparent() const;
	void updateShadedRectangleTransparency();
	void updateMessageTransparency();
	void updateMessageAlertTransparency();
	void setNewMessageAlert(bool _b);
	void resetTransparency();

	//View-related
	void  onResize(sf::Vector2u _newSize);
	void  snapToTop();
	void  snapToBottom();
	float getUpperViewBound() const;
	float getLowerViewBound() const;
	bool  viewAtHighest() const;
	bool  viewAtLowest() const;
	void  scrollUp();
	void  scrollDown();
	//---------------------------------------------

	//Data members --------------------------------
	mutable sf::RenderTarget* m_target;
	sf::Vector2u              m_lastTargetSize;

	const std::string m_name;

	sf::View           m_view;
	sf::View           m_shadedRectangleView;
	sf::RectangleShape m_shadedRectangle;

	std::vector<ChatBoxMessage> m_messages;
	Message                     m_lastMessage;
	spss::TextEntryBox          m_textEntry;
	//m_clock will be used to determine when the
	//messages in the chatbox will begin to fade
	//away.
	sf::Clock m_clock;

	bool m_anchoredToBottom;
	//---------------------------------------------
};

#endif // CHATBOX_H_INCLUDED
