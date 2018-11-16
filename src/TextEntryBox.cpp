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
     m_selectionBegin(0),
     m_selectionEnd(0),
     m_selectionDirection(0),
     m_text(),
     m_enteringText{false},
     m_inputComplete{false},
     m_alwaysVisible{false},
     m_alwaysActive{false},
     m_charSize{0},
     m_maxChars{_maxChars} {

    m_rectangle.setFillColor(sf::Color(0,0,0,120));
    m_highlightedRectangle.setFillColor(sf::Color(250,250,250,100));

    m_text.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));
    m_caret.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));
    m_caret.setColor(sf::Color(230,230,230));
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
                    if(!m_alwaysActive){
                        clearText();
                    }
                }

                m_enteringText = !m_enteringText;

                if(m_enteringText) {
                    InputLocker::get_instance().lock();
                }
                else {
                    InputLocker::get_instance().unlock();
                }
            }

            else if(keysPressedTogether({Key::TEXT_SELECTLEFT_A,
                                         Key::TEXT_SELECTLEFT_B})){
                selectLeft();
            }

            else if(keysPressedTogether({Key::TEXT_SELECTRIGHT_A,
                                         Key::TEXT_SELECTRIGHT_B})){
                selectRight();
            }

            else if(keysPressedTogether({Key::TEXT_SELECTALL_A,
                                         Key::TEXT_SELECTALL_B})){
                selectAll();
            }

            else if(keysPressedTogether({Key::TEXT_COPY_A,
                                         Key::TEXT_COPY_B})){
                //TODO: implement COPY functionality
                //requires SFML 2.5+
            }

            else if(keysPressedTogether({Key::TEXT_CUT_A,
                                         Key::TEXT_CUT_B})){
                //TODO: implement CUT functionality
                //requires SFML 2.5+
            }

            else if(keysPressedTogether({Key::TEXT_PASTE_A,
                                         Key::TEXT_PASTE_B})){
                //TODO: implement PASTE functionality
                //requires SFML 2.5+
            }

            else if(sf::Keyboard::isKeyPressed(Key::TEXT_MOVELEFT)){
                moveLeft();
            }

            else if(sf::Keyboard::isKeyPressed(Key::TEXT_MOVERIGHT)){
                moveRight();
            }



            break;
        }

        case sf::Event::TextEntered: {
            if(!m_enteringText) {
                break;
            }

            //Since typing "A" counts as text entry, we'll see if
            //CTRL is held too and not treat it as such if that's the
            //case. If it isn't, we'll delete the selection.
            if(sequenceSelected() &&
               !sf::Keyboard::isKeyPressed(Key::TEXT_SELECTALL_A)){
                    deleteSelection();
               }

            std::string newString{m_text.getString()};

            //8 = backspace
            //127 = delete
            if(_event.text.unicode == 8
               ||
               _event.text.unicode == 127
                && !stringEmpty()) {

                deleteSelection();
            }
            //Anything between 32 and 255
            //represents the rest of the
            //alphanumeric characters, and
            //some symbols
            else if(_event.text.unicode >= 32
                    &&
                    _event.text.unicode <= 255
                    &&
                    newString.length() < m_maxChars) {

                if(sequenceSelected()){
                    deleteSelection();
                }

                insert(static_cast<char>(_event.text.unicode));
                moveRight();
            }

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
    if(m_alwaysActive){
        m_enteringText = true;
    }
    if(m_enteringText) {
        updateHighlight();
        updateCaret();
        updateView();
    }
}

void TextEntryBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(m_enteringText || m_alwaysVisible) {
        target.setView(m_textView);
        target.draw(m_rectangle, states);
        target.draw(m_text, states);
        target.draw(m_highlightedRectangle, states);
    }
    if(m_enteringText && !sequenceSelected()){
        target.draw(m_caret, states);
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

void TextEntryBox::setActive(){
    m_enteringText = true;
}

void TextEntryBox::setInactive(){
    m_enteringText = false;
}

void TextEntryBox::setAlwaysVisible(bool _val){
    m_alwaysVisible = _val;
}

void TextEntryBox::setAlwaysActive(bool _val){
    m_alwaysActive = _val;
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

        //float caret_x{m_text.getGlobalBounds().width};
        //float caret_x{float(m_currentStringIndexPosition * m_charSize)};


        m_caret.setPosition({textXAtPosition(m_selectionBegin), m_caret.getPosition().y});
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

void TextEntryBox::updateHighlight(){
    if(sequenceSelected()){
        float startPos{textXAtPosition(m_selectionBegin)};
        float endPos{textXAtPosition(m_selectionEnd)};

        m_highlightedRectangle.setSize({endPos - startPos,
                                       m_rectangle.getGlobalBounds().height});

        m_highlightedRectangle.setPosition(startPos,
                                           m_text.getPosition().y);
    }
    else{
        m_highlightedRectangle.setSize({});
    }
}

bool TextEntryBox::stringEmpty() const {
    return m_text.getString().isEmpty();
}

void TextEntryBox::selectAll(){
    m_selectionBegin = 0;
    m_selectionEnd = m_text.getString().getSize();
    m_selectionDirection = 0;
}

void TextEntryBox::unselectAll(){
    m_selectionEnd = m_selectionBegin;
    m_selectionDirection = 0;
}

void TextEntryBox::selectLeft(){
    if(m_selectionDirection <= 0){
        if(m_selectionBegin <= 0){
            return;
        }
        --m_selectionBegin;
    }
    else{
        --m_selectionEnd;
    }
    --m_selectionDirection;
}

void TextEntryBox::selectRight(){
    if(m_selectionDirection >= 0){
        if(m_selectionEnd >= m_text.getString().getSize()){
            return;
        }
        ++m_selectionEnd;
    }
    else{
        ++m_selectionBegin;
    }
    ++m_selectionDirection;
}

void TextEntryBox::moveLeft(){
    if(!sequenceSelected() && m_selectionBegin > 0){
        --m_selectionBegin;
        m_selectionEnd = m_selectionBegin;
    }
    else if(sequenceSelected()){
        m_selectionEnd = m_selectionBegin;
    }
    m_selectionDirection = 0;
}

void TextEntryBox::moveRight(){
    if(!sequenceSelected() && m_selectionBegin < m_text.getString().getSize()){
        ++m_selectionBegin;
        m_selectionEnd = m_selectionBegin;
    }
    else if(sequenceSelected()){
        m_selectionBegin = m_selectionEnd;
    }
    m_selectionDirection = 0;
}

bool TextEntryBox::sequenceSelected() const{
    return (m_selectionBegin != m_selectionEnd);
    return (m_selectionBegin != m_selectionEnd);
}

void TextEntryBox::deleteSelection(){
    std::string newString{m_text.getString()};

    if(sequenceSelected()){
        newString.erase(m_selectionBegin,
                        m_selectionEnd - m_selectionBegin);
    }
    else{
        selectLeft();
        newString.erase(m_selectionBegin, 1);

    }

    m_selectionEnd = m_selectionBegin;
    m_selectionDirection = 0;
    m_text.setString(newString);
}

void TextEntryBox::insert(std::string& _str){
    std::string newString{m_text.getString()};
    newString.insert(m_selectionBegin,
                     _str);
    m_text.setString(newString);

    m_selectionBegin = m_selectionEnd;
    m_selectionDirection = 0;
}

void TextEntryBox::insert(char _char){
    std::string newString{m_text.getString()};
    newString.insert(m_selectionBegin,
                     1,
                     _char);
    m_text.setString(newString);

    m_selectionBegin = m_selectionEnd;
    m_selectionDirection = 0;
}

void TextEntryBox::clearText(){
    m_selectionBegin = 0;
    m_selectionEnd = 0;
    m_selectionDirection = 0;
    m_text.setString("");
    updateCaret();
}

float TextEntryBox::textXAtPosition(size_t _index){
    if(stringEmpty()){
        return 0;
    }

    float averageCharWidth{ m_text.getGlobalBounds().width /
                            m_text.getString().getSize()};

    return {float(_index
            * averageCharWidth)};
}
