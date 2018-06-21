#ifndef CHATBOX_H_INCLUDED
#define CHATBOX_H_INCLUDED

#include <SFML/Graphics.hpp>

class ChatBox : public sf::Drawable {
	public:
		ChatBox(sf::RenderWindow& _window);

		void appendMessage(const std::string _message, const std::string _sender = "");
		void getInput(sf::Event& _event);
		void update();
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

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
		void splitMessage(Message& _message);
		void positionMessage(Message& _message);
		void setTransparency(int _a);
		bool messagesTransparent() const;
		void updateShadedRectangleTransparency();
		void updateMessageTransparency();
		/////////////////////////////////////////

		//View-related
		/////////////////////////////////////////
		void onResize();
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
		sf::RenderWindow& m_window;

		sf::View m_view;
		sf::View m_shadedRectangleView;
		sf::View m_textEntryView;

		sf::RectangleShape m_shadedRectangle;
		sf::RectangleShape m_textEntryRectangle;

		std::vector<Message> m_messages;

		bool m_enteringText;
		sf::Clock m_clock;
};

#endif // CHATBOX_H_INCLUDED
