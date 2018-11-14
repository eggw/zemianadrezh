#include "TextEntryBox.h"

#include "Keybinds.h"

#include "FontManager.h"
#include "InputLocker.h"

const bool keysPressedTogether(std::vector<sf::Keyboard::Key> _keys){
    bool allPressed{true};
    for(auto& key : _keys){
        if(!sf::Keyboard::isKeyPressed(key)){
            allPressed = false;
            break;
        }
    }
    return allPressed;
}

constexpr int MAX_CHAR_SIZE{20};

TextEntryBox::TextEntryBox(sf::Vector2u _windowSize,
                           sf::FloatRect _viewPort,
                           unsigned int _maxChars)
    :m_textView(),
     m_rectangle(),
     m_text(),
     m_enteringText{false},
     m_inputComplete{false},
     m_allSelected{false},
     m_charSize{0},
     m_maxChars{_maxChars} {

    m_rectangle.setFillColor(sf::Color(0,0,0,120));
    m_highlightedRectangle.setFillColor(sf::Color(250,250,250,100));

    m_text.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));
    m_caret.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));

    m_caret.setString("|");

    m_textView.setViewport(_viewPort);
    onResize(_windowSize);
}

void TextEntryBox::getInput(sf::Event& _event) {
    switch(_event.type) {
        case sf::Event::KeyPressed: {
            if(_event.key.code == Key::CHAT_SEND) {
                if(m_enteringText && !stringEmpty()) {
                    m_inputComplete = true;
                    m_lastString = m_text.getString();
                    m_text.setString("");
                }
                m_enteringText = !m_enteringText;

                if(m_enteringText) {
                    InputLocker::get_instance().lock();
                }
                else {
                    InputLocker::get_instance().unlock();
                }
            }

            else if(keysPressedTogether({Key::TEXT_SELECTALL_A,
                                         Key::TEXT_SELECTALL_A})){
                selectAll();
            }

            break;
        }

        case sf::Event::TextEntered: {
            if(!m_enteringText) {
                break;
            }

            if(m_allSelected && !sf::Keyboard::isKeyPressed(Key::TEXT_SELECTALL_A)){
                unselectAll();
                m_text.setString("");
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

        case sf::Event::Resized: {
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
        if(m_allSelected){
            selectAll();
        }
    }
}

void TextEntryBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(m_enteringText) {
        target.setView(m_textView);
        target.draw(m_rectangle, states);
        target.draw(m_text, states);
        target.draw(m_caret, states);
        target.draw(m_highlightedRectangle, states);
    }
}

bool TextEntryBox::enteringText() const {
    return m_enteringText;
}

bool TextEntryBox::inputComplete() {
    if(m_inputComplete) {
        m_inputComplete = false;
        return true;
    }
    return false;
}

std::string TextEntryBox::getLastString() const {
    return m_lastString;
}

void TextEntryBox::onResize(sf::Vector2u _newSize) {
    float xRatio{m_textView.getViewport().width};
    float yRatio{m_textView.getViewport().height};

    m_textView.reset({0,
                      0,
                      _newSize.x * xRatio,
                      _newSize.y * yRatio
                     });


    sf::Vector2f rectSize{m_textView.getSize()};
    m_rectangle.setSize(rectSize);

    m_charSize = m_rectangle.getSize().y - 4;
    if(m_charSize > MAX_CHAR_SIZE) {
        m_charSize = MAX_CHAR_SIZE;
    }

    m_text.setCharacterSize(m_charSize);
    m_caret.setCharacterSize(m_charSize);

    float lineSpacing {FontManager::get_instance().getLineSpacing(FontManager::Type::ANDY, m_charSize)};
    double textHeight{m_text.getPosition().y + lineSpacing};
    double rectangleHeight{m_rectangle.getGlobalBounds().height * 0.8};

    if(textHeight > rectangleHeight) {
        double difference{textHeight - rectangleHeight};
        sf::Vector2f newTextPosition{m_text.getPosition()};
        newTextPosition.y -= difference;
        m_text.setPosition(newTextPosition);
    }

    if(m_allSelected){
        selectAll();
    }
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
    sf::Vector2f newRectangleSize{m_rectangle.getSize()};

    if(m_text.getGlobalBounds().width > m_textView.getSize().x) {
        sf::Vector2f newCenter{m_textView.getCenter()};
        newCenter.x = m_text.getGlobalBounds().width;
        newCenter.x -= m_textView.getSize().x / 2;
        newCenter.x += m_caret.getGlobalBounds().width * 2;
        m_textView.setCenter(newCenter);

        newRectangleSize.x +=   m_text.getGlobalBounds().width +
                                m_caret.getGlobalBounds().width;
    }
    else {
        sf::Vector2f originalCenter{m_textView.getSize().x / 2,
                                    m_textView.getCenter().y};
        m_textView.setCenter(originalCenter);

        newRectangleSize.x = m_textView.getSize().x;
    }

    m_rectangle.setSize(newRectangleSize);

}

bool TextEntryBox::stringEmpty() const {
    return m_text.getString().isEmpty();
}

void TextEntryBox::selectAll(){
    m_allSelected = true;
    sf::Vector2f size{  m_text.getGlobalBounds().width + m_caret.getGlobalBounds().width,
                        m_rectangle.getGlobalBounds().height};
    m_highlightedRectangle.setSize(size);
    m_highlightedRectangle.setPosition(m_text.getPosition());
    m_text.setColor(sf::Color::Yellow);
}

void TextEntryBox::unselectAll(){
    m_allSelected = false;
    m_highlightedRectangle.setSize({});
    m_text.setColor(sf::Color::White);
}
