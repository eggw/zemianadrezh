#include "ChatBox.h"

#include "FontManager.h"

#include "Keybinds.h"

#include <iostream>

constexpr float X_WINDOW_RATIO{0.3};
constexpr float Y_WINDOW_RATIO{0.25};

const unsigned int CHARACTER_SIZE {20};
const float Y_OFFSET = FontManager::get_instance().getLineSpacing(FontManager::Type::ANDY, CHARACTER_SIZE);
const float Y_BUFFERSPACE{2.3f * Y_OFFSET};

//The value of 2.3 ABOVE is because the origin of the text's position is top
//left; in order to get an empty row below, we're going to need to subtract
//2 * Y_OFFSET. The .3 is for a little aesthetic extra buffer space below.

ChatBox::ChatBox(sf::RenderWindow& _window)
    :m_window(_window),
     m_view( {
    sf::FloatRect({0}, {0}, m_window.getSize().x * X_WINDOW_RATIO, m_window.getSize().y * Y_WINDOW_RATIO)
}),
m_shadedRectangleView(m_view),
m_shadedRectangle(),
m_messages(),
m_enteringText{false},
m_clock() {

    m_shadedRectangle.setSize(m_view.getSize());
    m_view.setViewport({0, 0.75, X_WINDOW_RATIO, Y_WINDOW_RATIO});
    m_shadedRectangleView.setViewport({0, 0.75, X_WINDOW_RATIO, Y_WINDOW_RATIO});

    appendMessage("Impending doom approaches... Also this is a test to see if the splitter function works properly");
    appendMessage("message1", "Test");
    appendMessage("message2", "Test");
    appendMessage("message3", "Test");
    appendMessage("message4", "Test");
    appendMessage("message5", "Test");

    appendMessage("message6", "Test");
    appendMessage("message7", "Test");
    appendMessage("message8", "Test");
    appendMessage("message9", "Test");

}

void ChatBox::appendMessage(const std::string _message, const std::string _sender) {
    sf::Text newText;
    newText.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));
    newText.setCharacterSize(CHARACTER_SIZE);

    std::string finalStr{_message};
    if(_sender != "") {
        finalStr = ("<" + _sender + "> " + finalStr);
    }
    else {
        newText.setColor(sf::Color::Red);
    }
    newText.setString(finalStr);

    Message newMessage{newText};

    if(messageTooWide(newMessage)) {
        splitMessage(newMessage);
    }

    positionMessage(newMessage);

    m_messages.push_back(newMessage);
    snapToBottom();
    m_clock.restart();
}

void ChatBox::getInput(sf::Event& _event) {
    //In the case of the enter key, we're going to poll an event.
    //The reason for this is that we only want to toggle this once,
    //and not have it rapidly be called every frame.

    switch(_event.type) {
        case sf::Event::KeyPressed: {
            if(_event.key.code == Key::CHAT_SEND) {
                m_enteringText = !m_enteringText;
                snapToBottom();
                m_clock.restart();
            }

            else if(_event.key.code == Key::CHAT_UP) {
                m_clock.restart();
                scrollUp();
            }

            else if(_event.key.code == Key::CHAT_DOWN) {
                m_clock.restart();
                scrollDown();
            }
            break;
        }
        default:
            break;
    }

}

void ChatBox::update() {
    updateShadedRectangleTransparency();
    updateMessageTransparency();
}

void ChatBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::View previousView = target.getView();
    target.setView(m_shadedRectangleView);
    target.draw(m_shadedRectangle, states);
    target.setView(m_view);
    if(!messagesTransparent()) {
        for(auto& message : m_messages) {
            target.draw(message.text, states);
        }
    }

    target.setView(previousView);
}

const bool ChatBox::messageTooWide(Message& _message) const {
    return _message.text.getGlobalBounds().width >= m_view.getSize().x * 0.9;
}

void ChatBox::splitMessage(Message& _message) {
    std::string textStr = _message.text.getString();
    float charSize = _message.text.getGlobalBounds().width / textStr.size();
    auto widthLimit = m_view.getSize().x * 0.9;

    float currentWidth{0};

    for(size_t i{0}; i < textStr.length(); i++) {
        currentWidth += charSize;

        //Insert a newline at the position, if it's
        //too wide
        if(currentWidth >= widthLimit) {
            textStr.insert(i, 1, '\n');
            _message.numberOfLines++;
            currentWidth = 0;
        }

        //If we have a space as the first character
        //on a new line, we'll get rid of it
        else if(currentWidth == charSize) {
            if(isspace(textStr.at(i))) {
                textStr.erase(i, 1);
            }
        }
    }

    _message.text.setString(textStr);
}

void ChatBox::positionMessage(Message& _message) {
    if(!m_messages.empty()) {
        sf::Vector2f newPosition{m_messages.back().text.getPosition()};
        unsigned int lastMessageLines{m_messages.back().numberOfLines};
        newPosition.y += lastMessageLines * (Y_OFFSET);
        _message.text.setPosition(newPosition);
    }
}

void ChatBox::setTransparency(int _a) {
    for(auto& message : m_messages) {
        sf::Color color = message.text.getFillColor();
        color.a = _a;
        message.text.setFillColor(color);
    }
}

bool ChatBox::messagesTransparent() const {
    if(!m_messages.empty()) {
        return m_messages.front().text.getColor().a == 0;
    }
}

void ChatBox::updateShadedRectangleTransparency() {
    static int rectangleAlphaValue{0};

    if(m_enteringText) {
        rectangleAlphaValue = 100;
    }
    else {
        if(rectangleAlphaValue > 0) {
            rectangleAlphaValue -= 1;
        }
    }

    m_shadedRectangle.setFillColor(sf::Color(0,0,0,rectangleAlphaValue));
}

void ChatBox::updateMessageTransparency() {
    static int textAlphaValue{255};

    if(m_enteringText) {
        textAlphaValue = 255;
    }
    else {
        if(m_clock.getElapsedTime().asSeconds() > 3) {
            if(textAlphaValue > 0) {
                textAlphaValue -= 1;
            }
        }
        //The clock might have been reset due to the user scrolling
        //up or down, despite m_enteringText being false.
        //In this case, we still want the text to be visible.
        else {
            textAlphaValue = 255;
        }
    }

    setTransparency(textAlphaValue);
}


//This function checks if the last message is "outside" (below) the view.
//If so, it adjusts the view's center so that the very first message is
//on top.
void ChatBox::snapToTop() {
    if(!m_messages.empty()) {
        Message& latestMessage = m_messages.back();
        float boundary{m_view.getSize().y - Y_OFFSET};

        if(latestMessage.text.getPosition().y < boundary) {
            return;
        }

        Message& earliestMessage = m_messages.front();
        sf::Vector2f newCenter {m_view.getCenter().x, earliestMessage.text.getPosition().y};

        newCenter.y += (m_view.getSize().y / 2);

        m_view.setCenter(newCenter);
    }
}

//This function checks if the last message is "outside" (below) the view.
//If so, it adjusts the view's center so that it is visible.
void ChatBox::snapToBottom() {
    if(!m_messages.empty()) {
        Message& latestMessage = m_messages.back();
        float boundary{m_view.getSize().y - Y_OFFSET};

        if(latestMessage.text.getPosition().y < boundary) {
            return;
        }

        sf::Vector2f newCenter {m_view.getCenter().x, latestMessage.text.getPosition().y};

        newCenter.y -= (m_view.getSize().y / 2) - (Y_BUFFERSPACE);

        m_view.setCenter(newCenter);
    }
}

float ChatBox::getUpperViewBound() const {
    return 0;
}
float ChatBox::getLowerViewBound() const {
    float lowestPoint{m_messages.back().text.getPosition().y};
    lowestPoint += Y_BUFFERSPACE;
    return lowestPoint;
}

bool ChatBox::viewAtHighest() const {
    if(m_view.getCenter().y - m_view.getSize().y / 2 <= getUpperViewBound()) {
        return true;
    }

    return false;
}

bool ChatBox::viewAtLowest() const {
    if(m_view.getCenter().y + m_view.getSize().y / 2 >= getLowerViewBound()) {
        return true;
    }

    return false;
}

void ChatBox::scrollUp() {
    if(!viewAtHighest()) {
        m_view.setCenter(m_view.getCenter().x, m_view.getCenter().y - (Y_OFFSET / 5));
    }
    if(viewAtHighest()){
        snapToTop();
    }
}

void ChatBox::scrollDown() {
    if(!viewAtLowest()) {
        m_view.setCenter(m_view.getCenter().x, m_view.getCenter().y + (Y_OFFSET / 5));
    }
    if(viewAtLowest()){
        snapToBottom();
    }
}


