#ifndef TEXTENTRYBOX_H_INCLUDED
#define TEXTENTRYBOX_H_INCLUDED

#include <SFML/Graphics.hpp>

class TextEntryBox : public sf::Drawable {
    public:
        TextEntryBox(sf::Vector2u _windowSize,
                     sf::FloatRect _viewPort,
                     unsigned int _maxChars = 255);

        void getInput(sf::Event& _event);
        void update();
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        bool enteringText() const;
        bool inputComplete();

        std::string getLastString() const;

        void setActive();
        void setInactive();
        void setAlwaysVisible(bool _val);
        void setAlwaysActive(bool _val);
    private:
        void onResize(sf::Vector2u _newSize);
        void updateCaret();
        void updateView();
        void updateHighlight();
        bool stringEmpty() const;
        void selectAll();
        void unselectAll();

        void moveLeft();
        void moveRight();

        void selectLeft();
        void selectRight();

        bool sequenceSelected() const;

        void deleteSelection();
        void insert(std::string& _str);
        void insert(char _char);

        void clearText();

        float textXAtPosition(size_t _index);
    private:
        sf::View m_textView;
        sf::RectangleShape m_rectangle;
        sf::RectangleShape m_highlightedRectangle;

        size_t m_selectionBegin;
        size_t m_selectionEnd;
        int m_selectionDirection;

        sf::Text m_text;
        sf::Text m_caret;
        bool m_enteringText;
        bool m_inputComplete;
        //m_alwaysVisible will simply display
        //the box at all times, even when
        //m_enteringText is false
        bool m_alwaysVisible;
        //m_alwaysActive will ensure
        //m_enteringText is always set to
        //true; additionally, it prevents text
        //from being cleared upon pressing Enter
        bool m_alwaysActive;

        std::string m_lastString;

        int m_charSize;
        unsigned int m_maxChars;
};

#endif // TEXTENTRYBOX_H_INCLUDED
