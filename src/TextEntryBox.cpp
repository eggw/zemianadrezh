#include "TextEntryBox.h"

#include "Keybinds.h"

#include "FontManager.h"
#include <iostream>
#include "InputLocker.h"

TextEntryBox::TextEntryBox(Util::WindowInfo _windowInfo,
                           int _charSize,
                           unsigned int _maxChars)
    :m_textView(),
     m_rectangleView(),
     m_rectangle(),
     m_text(),
     m_enteringText{false},
     m_inputComplete{false},
     m_windowInfo{_windowInfo},
     m_charSize{_charSize},
     m_maxChars{_maxChars} {

    m_rectangle.setFillColor(sf::Color(0,0,0,120));

    m_text.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));
    m_caret.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));
    m_text.setCharacterSize(m_charSize);
    m_caret.setCharacterSize(m_charSize);

    m_caret.setString("|");

    onResize(m_windowInfo.size);
}

void TextEntryBox::getInput(sf::Event& _event) {
    switch(_event.type) {
        case sf::Event::KeyPressed: {
            if(_event.key.code == Key::CHAT_SEND) {
                if(m_enteringText && !stringEmpty()){
                    m_inputComplete = true;
                    m_lastString = m_text.getString();
                    m_text.setString("");
                }
                m_enteringText = !m_enteringText;

                if(m_enteringText){
                    InputLocker::get_instance().lock();
                }
                else{
                    InputLocker::get_instance().unlock();
                }
            }
            break;
        }

        case sf::Event::TextEntered: {
            if(!m_enteringText) {
                break;
            }

            std::string newString{m_text.getString()};

            if(_event.text.unicode == 8 && !stringEmpty()) {
                newString.erase(newString.size()-1);
            }
            else if(_event.text.unicode >= 32
                    &&
                    _event.text.unicode <= 255
                    &&
                    newString.length() < m_maxChars) {

                newString += static_cast<char>(_event.text.unicode);
            }

            m_text.setString(newString);
            break;
        }

        case sf::Event::Resized:{
            sf::Vector2u newSize{_event.size.width, _event.size.height};
            onResize(newSize);
            break;
        }

        default:
            break;

    }

}

void TextEntryBox::update() {
    if(m_enteringText) {
        updateCaret();
        updateView();
    }
}

void TextEntryBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(m_enteringText) {
        target.setView(m_rectangleView);
        target.draw(m_rectangle, states);

        target.setView(m_textView);
        target.draw(m_text, states);
        target.draw(m_caret, states);
    }
}

bool TextEntryBox::enteringText() const {
    return m_enteringText;
}

bool TextEntryBox::inputComplete() {
    if(m_inputComplete){
        m_inputComplete = false;
        return true;
    }
    return false;
}

std::string TextEntryBox::getLastString() const {
    return m_lastString;
}

void TextEntryBox::onResize(sf::Vector2u _newSize) {

    m_textView.reset({0,
                      0,
                      _newSize.x * m_windowInfo.ratio.x,
                      _newSize.y * m_windowInfo.ratio.y
                     });


    m_textView.setViewport({m_windowInfo.position.x / m_windowInfo.size.x,
                            m_windowInfo.position.y / m_windowInfo.size.y,
                            m_windowInfo.ratio.x,
                            m_windowInfo.ratio.y
                           });

    sf::Vector2f rectSize{m_textView.getSize()};
    m_rectangle.setSize(rectSize);

    m_rectangleView = m_textView;

}

void TextEntryBox::updateCaret() {
    if(m_enteringText) {
        static bool caretAlphaDecreasing{true};
        static int caretAlphaValue{255};

        if(caretAlphaDecreasing) {
            caretAlphaValue -= 5;
            if(caretAlphaValue <= 0) {
                caretAlphaDecreasing = false;
            }
        }
        else {
            caretAlphaValue += 5;
            if(caretAlphaValue >= 255) {
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

void TextEntryBox::updateView() {
    if(m_text.getGlobalBounds().width > m_textView.getSize().x) {
        //std::cout << "wide" << std::endl;
        //set to position of caret minus half the view width minus charsize
        sf::Vector2f newCenter{m_textView.getCenter()};
        newCenter.x = m_text.getGlobalBounds().width;
        newCenter.x -= m_textView.getSize().x / 2;
        newCenter.x += m_caret.getGlobalBounds().width * 2;
        m_textView.setCenter(newCenter);
    }
    else {
        sf::Vector2f originalCenter{m_textView.getSize().x / 2,
                                    m_textView.getCenter().y};
        m_textView.setCenter(originalCenter);
    }
}

bool TextEntryBox::stringEmpty() const{
    return m_text.getString().isEmpty();
}
