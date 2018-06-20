#include "ChatBox.h"

#include "FontManager.h"

#include "Keybinds.h"

#include <iostream>

const unsigned int CHARACTER_SIZE {19};
const float Y_OFFSET = FontManager::get_instance().getLineSpacing(FontManager::Type::ANDY, CHARACTER_SIZE);

ChatBox::ChatBox(sf::RenderWindow& _window)
    :m_window(_window),
     m_view({sf::FloatRect({0}, {0}, m_window.getSize().x * 0.4, m_window.getSize().y * 0.25)}),
     m_messages() {


    m_view.setViewport({0, 0.75, 0.4, 0.25});
    appendMessage("Impending doom approaches... Also this is a test to see if the splitter function works properly");
    appendMessage("message1", "Test");
    appendMessage("message2", "Test");
    appendMessage("message3", "Test");
    appendMessage("message4", "Test");
    appendMessage("message5", "Test");
    appendMessage("message6", "Test");
	appendMessage("message7", "Test");
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
        newText.setColor(sf::Color::Green);
    }
    newText.setString(finalStr);

    Message newMessage{newText};

    if(messageTooWide(newMessage)) {
        splitMessage(newMessage);
    }

    positionMessage(newMessage);

    m_messages.push_back(newMessage);
    //updateView();
}

void ChatBox::getInput() {
    if(sf::Keyboard::isKeyPressed(Key::CHAT_UP)) {
		std::cout << "view size is " << m_view.getSize().x << ", "<< m_view.getSize().y << std::endl;
		if(!viewAtHighest()){
			m_view.setCenter(m_view.getCenter().x, m_view.getCenter().y - 5);
		}
    }

    if(sf::Keyboard::isKeyPressed(Key::CHAT_DOWN)) {
		std::cout << "view size is " << m_view.getSize().x << ", "<< m_view.getSize().y << std::endl;
		if(!viewAtLowest()){
			m_view.setCenter(m_view.getCenter().x, m_view.getCenter().y + 5);
		}
    }
}

void ChatBox::update() {

}

void ChatBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::View previousView = target.getView();
    target.setView(m_view);
    for(auto& message : m_messages) {
        target.draw(message.text, states);
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


void ChatBox::updateView() {
    if(!m_messages.empty()) {
        Message& message = m_messages.back();

        float viewHeight = getViewHeight();

        if(message.text.getPosition().y - message.text.getGlobalBounds().height * 2 > viewHeight) {
            sf::Vector2f newCenter = m_view.getCenter();
            newCenter.y += message.numberOfLines * (Y_OFFSET);
            m_view.setCenter(newCenter);
        }
    }
}

float ChatBox::getViewHeight() const {
    return (1 - m_view.getViewport().top)
           *
           m_window.getSize().y;
}

bool ChatBox::viewAtHighest() const {
    float viewHeight = getViewHeight();

    std::cout << "Up\t";
    std::cout << m_view.getCenter().y - viewHeight / 2;
    std::cout << " is ";

    if(m_view.getCenter().y - viewHeight / 2 >= 0) {
		std::cout << ">= 0; failure!" << std::endl;
        return true;
    }

    std::cout << "< 0; success" << std::endl;

    return false;
}

bool ChatBox::viewAtLowest() const {
    float viewHeight = getViewHeight();

    std::cout << "Down\t";
    std::cout << m_view.getCenter().y + viewHeight / 2;
    std::cout << " is ";

    if(m_view.getCenter().y + viewHeight / 2 >= m_messages.back().text.getPosition().y) {
        std::cout << ">= " << m_messages.back().text.getPosition().y <<"; failure!" << std::endl;
        return true;
    }

    std::cout << "< " << m_messages.back().text.getPosition().y <<"; success!" << std::endl;

    return false;
}



