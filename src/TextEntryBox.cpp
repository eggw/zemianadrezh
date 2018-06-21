#include "TextEntryBox.h"

#include "Keybinds.h"

#include "FontManager.h"

TextEntryBox::TextEntryBox(sf::Window& _window, int _charSize, float _xWindowRatio, float _height)
    :m_view(),
     m_rectangle(),
     m_text(),
     m_enteringText(false),
     m_xWindowRatio{_xWindowRatio},
     m_height{_height} {

    m_rectangle.setFillColor(sf::Color(0,0,0,120));

    m_text.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));
    m_caret.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));
    m_text.setCharacterSize(_charSize);
    m_caret.setCharacterSize(_charSize);

    m_caret.setString("|");

    onResize(_window.getSize());
}

void TextEntryBox::getInput(sf::Event& _event) {
    switch(_event.type) {
        case sf::Event::KeyPressed: {
            if(_event.key.code == Key::CHAT_SEND) {
                m_enteringText = !m_enteringText;
            }
        }
    }

}

void TextEntryBox::update() {

}

void TextEntryBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(m_enteringText) {
        target.setView(m_view);
        target.draw(m_rectangle, states);
    }
}

bool TextEntryBox::enteringText() const {
    return m_enteringText;
}

void TextEntryBox::onResize(sf::Vector2u _newSize) {
    float yPos   {((_newSize.y - m_height)
                          /
                          (_newSize.y))};

    m_view.reset({0,
                  0,
                  _newSize.x * m_xWindowRatio,
                  m_height
                 });

    m_view.setViewport({0,
                        yPos,
                        m_xWindowRatio,
                        1 - yPos
                       });

    sf::Vector2f rectSize{m_view.getSize()};
    m_rectangle.setSize(rectSize);

}

