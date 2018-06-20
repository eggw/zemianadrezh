#ifndef CHATBOX_H_INCLUDED
#define CHATBOX_H_INCLUDED

#include <SFML/Graphics.hpp>

class ChatBox : public sf::Drawable {
    public:
        ChatBox(sf::RenderWindow& _window);

        void appendMessage(const std::string _message, const std::string _sender = "");
        void getInput();
        void update();
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
    	struct Message{
    		Message(sf::Text& _text, unsigned int _numberOfLines = 1)
						:text{_text},
						numberOfLines{_numberOfLines}{
					}

			sf::Text text;
			unsigned int numberOfLines;
    	};

    	//Message-related
    	/////////////////////////////////////////
    	const bool messageTooWide(Message& _message, float _widthRatio) const;
    	void splitMessage(Message& _message, float _widthRatio);
    	void positionMessage(Message& _message);
    	/////////////////////////////////////////

    	//View-related
    	/////////////////////////////////////////
    	void updateView();
		float getViewHeight() const;
    	bool viewAtHighest() const;
    	bool viewAtLowest() const;
    	/////////////////////////////////////////

	private:
    	sf::RenderWindow& m_window;
    	sf::View m_view;
    	std::vector<Message> m_messages;
};

#endif // CHATBOX_H_INCLUDED
