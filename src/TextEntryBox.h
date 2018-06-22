#ifndef TEXTENTRYBOX_H_INCLUDED
#define TEXTENTRYBOX_H_INCLUDED

#include <SFML/Graphics.hpp>

class TextEntryBox : public sf::Drawable {
    public:
        TextEntryBox(sf::Window& _window,
					int _charSize,
					float _xWindowRatio,
					float _height,
					unsigned int _maxChars = 255);

        void getInput(sf::Event& _event);
        void update();
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        bool enteringText() const;
        bool inputComplete();

        std::string getLastString() const;
    private:
		void onResize(sf::Vector2u _newSize);
		void updateCaret();
		void updateView();
		bool stringEmpty() const;
    private:
    	sf::View m_textView;
    	sf::View m_rectangleView;
    	sf::RectangleShape m_rectangle;
    	sf::Text m_text;
    	sf::Text m_caret;
    	bool m_enteringText;
    	bool m_inputComplete;

    	std::string m_lastString;

    	int m_charSize;
    	float m_xWindowRatio;
    	float m_height;
    	unsigned int m_maxChars;
};

#endif // TEXTENTRYBOX_H_INCLUDED
