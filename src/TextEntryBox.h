#ifndef TEXTENTRYBOX_H_INCLUDED
#define TEXTENTRYBOX_H_INCLUDED

#include <SFML/Graphics.hpp>

class TextEntryBox : public sf::Drawable {
    public:
        TextEntryBox(sf::Window& _window, int _charSize, float _xWindowRatio, float _height);
        void getInput(sf::Event& _event);
        void update();
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        bool enteringText() const;
    private:
		void onResize(sf::Vector2u _newSize);
		void updateCaret();
		void updateView();
    private:
    	sf::View m_textView;
    	sf::View m_rectangleView;
    	sf::RectangleShape m_rectangle;
    	sf::Text m_text;
    	sf::Text m_caret;
    	bool m_enteringText;

    	int m_charSize;
    	float m_xWindowRatio;
    	float m_height;
};

#endif // TEXTENTRYBOX_H_INCLUDED
