#ifndef CHATBOX_H_INCLUDED
#define CHATBOX_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "TextEntryBox.h"

class ChatBox : public sf::Drawable {
	public:
		ChatBox(sf::RenderWindow& _window,
				const std::string& _name);

		void appendMessage(	const std::string _message,
							const std::string _sender = "");
		void getInput(sf::Event& _event);
		void update();
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		bool completedMessage(std::pair<std::string, std::string>* _ptr = nullptr);
	private:
		struct Message {
			Message(sf::Text& _text, unsigned int _numberOfLines = 1)
				:text{_text},
				 numberOfLines{_numberOfLines} {
			}

			sf::Text text;
			unsigned int numberOfLines;
		};

		//Message-related
		/////////////////////////////////////////
		const bool messageTooWide(Message& _message) const;
		const bool messageTooNarrow(Message& _message) const;
		void adjustMessage(Message& _message);
		void positionMessage(int _index);
		void setTransparency(int _a);
		bool messagesTransparent() const;
		void updateShadedRectangleTransparency();
		void updateMessageTransparency();
		/////////////////////////////////////////

		//View-related
		/////////////////////////////////////////
		void onResize(sf::Vector2u _newSize);
		void snapToTop();
		void snapToBottom();
		float getUpperViewBound() const;
		float getLowerViewBound() const;
		bool viewAtHighest() const;
		bool viewAtLowest() const;
		void scrollUp();
		void scrollDown();
		/////////////////////////////////////////

	private:
		const std::string& m_name;

		sf::View m_view;
		sf::View m_shadedRectangleView;

		sf::RectangleShape m_shadedRectangle;

		std::vector<Message> m_messages;
		std::pair<std::string, std::string> m_lastMessage;

		TextEntryBox m_textEntry;
		sf::Clock m_clock;
};

#endif // CHATBOX_H_INCLUDED
