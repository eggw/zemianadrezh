#include "TextEntryBox.h"

#include "Keybinds.h"

#include "FontManager.h"

TextEntryBox::TextEntryBox(sf::Window& _window, int _charSize, float _xWindowRatio, float _height)
    :m_view(),
     m_rectangle(),
     m_text(),
     m_enteringText(false),
     m_charSize{_charSize},
     m_xWindowRatio{_xWindowRatio},
     m_height{_height} {

    m_rectangle.setFillColor(sf::Color(0,0,0,120));

    m_text.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));
    m_caret.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));
    m_text.setCharacterSize(m_charSize);
    m_caret.setCharacterSize(m_charSize);

    m_caret.setString("|");

    onResize(_window.getSize());
}

void TextEntryBox::getInput(sf::Event& _event) {
    switch(_event.type) {
        case sf::Event::KeyPressed: {
            if(_event.key.code == Key::CHAT_SEND) {
                m_enteringText = !m_enteringText;
            }
            break;
        }

        case sf::Event::TextEntered: {
            if(!m_enteringText){
                break;
            }

            std::string newString{m_text.getString()};

            if(_event.text.unicode == 8 && !m_text.getString().isEmpty()){
                newString.erase(newString.size()-1);
            }
            else if(_event.text.unicode >= 32
                    &&
                    _event.text.unicode <= 255){

                newString += static_cast<char>(_event.text.unicode);
            }

            m_text.setString(newString);
            break;
        }


        default:
            break;

    }

}

void TextEntryBox::update() {
    updateCaret();
}

void TextEntryBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(m_enteringText) {
        target.setView(m_view);
        target.draw(m_rectangle, states);
        target.draw(m_text, states);
        target.draw(m_caret, states);
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

void TextEntryBox::updateCaret(){
    if(m_enteringText){
        static bool caretAlphaDecreasing{true};
        static int caretAlphaValue{255};

        if(caretAlphaDecreasing){
            caretAlphaValue -= 5;
            if(caretAlphaValue <= 0){
                caretAlphaDecreasing = false;
            }
        }
        else{
            caretAlphaValue += 5;
            if(caretAlphaValue >= 255){
                caretAlphaDecreasing = true;
            }
        }

        sf::Color caretColor = m_caret.getColor();
        caretColor.a = caretAlphaValue;
        m_caret.setColor(caretColor);

        float caret_x{m_text.getGlobalBounds().width};
        m_caret.setPosition({caret_x, m_caret.getPosition().y});
    }
}
