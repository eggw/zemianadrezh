#include "Interface/ChatBox.h"

#include "FontManager.h"
#include "InputLocker.h"
#include "Keybinds.h"

#include <iostream>

//Here are the variables we'll use to initialise the text entry box
//in the correct position
//Note that the height (0.25 by default) specifies that of both the
//ChatBox and m_textEntry combined.
//////////////////////////////////////////////////////////////////////////////////////////////
const sf::FloatRect    VIEWPORT{0, 0.75f, 0.3f, 0.25f};
constexpr float        SECONDS_UNTIL_MESSAGES_FADE{5.0f};
constexpr unsigned int CHARACTER_SIZE{20};

const float LINESPACING =
  FontManager::get_instance().get(FONT::ANDY)->getLineSpacing(CHARACTER_SIZE);
const float Y_BUFFERSPACE{1.3f * LINESPACING};

//The value of 1.3 above is because the origin of the text's position is top
//left, in order to get a little aesthetic extra buffer space below the
//last message
//////////////////////////////////////////////////////////////////////////////////////////////

ChatBox::ChatBox(const std::string& _name)
            : m_target{nullptr},
              m_lastTargetSize{},
              m_name{_name},
              m_infoBox{{0, 0}, {0, 0}, *FontManager::get_instance().get(FONT::ANDY)},
              m_view{},
              m_lastMessage{},
              m_textEntry{*FontManager::get_instance().get(FONT::ANDY),
                          CHARACTER_SIZE,
                          0},
              m_clock{},
              m_anchoredToBottom{true} {
	//snapToBottom();
	m_textEntry.setOutlineThickness(1);
	m_textEntry.setOutlineColor(sf::Color::Black);
}

void ChatBox::appendMessage(const spss::Message& _msg) {
	m_infoBox.appendMessage(_msg);
	//resetTransparency();

	if (m_anchoredToBottom) {
		//snapToBottom();
	}

	//If we get a new message and we're not anchored to the bottom, we make sure to
	//enable the visual notification of a new unviewed message
	else {
		//setNewMessageAlert(true);
	}
}

void ChatBox::getInput(sf::Event& _event) {
	//In the case of the enter key, we're going to poll an event.
	//The reason for this is that we only want to toggle this once,
	//and not have it rapidly be called every frame.

	m_infoBox.getInput(_event);
	m_textEntry.getInput(_event);

	switch (_event.type) {
	case sf::Event::KeyPressed: {
		if (_event.key.code == Key::CHAT_SEND) {
			if (m_textEntry.enteringText()) {
				m_textEntry.setActive(false);
				InputLocker::get_instance().unlock();
			}
			else {
				m_textEntry.setActive(true);
				InputLocker::get_instance().lock();
			}
			//resetTransparency();
		}

		else if (_event.key.code == Key::CHAT_UP) {
			//scrollUp();
		}

		else if (_event.key.code == Key::CHAT_DOWN) {
			//scrollDown();
		}

		else if (_event.key.code == Key::CHAT_TOP) {
			//snapToTop();
		}

		else if (_event.key.code == Key::CHAT_BOTTOM) {
			//snapToBottom();
		}

		break;
	}

	default:
		break;
	}
}

void ChatBox::update() {
	//onResize() will need to be called at least once after
	//m_target is set, so this code will be needed here.
	//As a result, we also won't really need to detect
	//a Resized event in getInput(), since this part
	//will handle that as well.
	if (m_target != nullptr) {
		if (m_target->getSize() != m_lastTargetSize) {
			m_lastTargetSize = m_target->getSize();
			onResize(m_lastTargetSize);
		}
	}

	m_infoBox.update();
	m_textEntry.update();

	if (m_textEntry.inputComplete()) {
		m_lastMessage = {m_name, m_textEntry.getLastString()};
	}
	if (m_textEntry.enteringText()) {
		//resetTransparency();
	}
	//updateShadedRectangleTransparency();
	//updateMessageTransparency();
	//updateMessageAlertTransparency();
}

void ChatBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	m_target              = &target;
	sf::View previousView = m_target->getView();

	m_target->setView(m_view);
	//if (!messagesTransparent()) {
		m_target->draw(m_infoBox, states);
		m_target->draw(m_textEntry, states);
	//}

	m_target->setView(previousView);
}

bool ChatBox::completedMessage(spss::Message* _ptr) {
	if (m_lastMessage.title == "" && m_lastMessage.content == "") {
		return false;
	}

	*_ptr                 = m_lastMessage;
	m_lastMessage.title  = "";
	m_lastMessage.content = "";

	return true;
}


void ChatBox::onResize(sf::Vector2u _newSize) {
	m_view = sf::View({float(_newSize.x / 2), float(_newSize.y / 2)},
			 {float(_newSize.x), float(_newSize.y)});

	//First, we'll get the text entry height as a ratio.
	//For example, if the window height is 1000p and the
	//text entry is 100p, textEntryHeight will be 0.1
	float textEntryHeight{1 - (_newSize.y - m_textEntry.getHeight()) / _newSize.y};

	//We'll then subtract that the entire ChatBox height.
	//For example, if the ChatBox's viewport height is 0.25 (it takes
	//up a quarter of the vertical space), and textEntryHeight is 0.1,
	//heightWithoutTextEntry would be 0.15.
	float heightWithoutTextEntry{VIEWPORT.height - textEntryHeight};

	//TODO: error checking if heightWithoutTextEntry is negative.

	//snapToBottom();

	const sf::Vector2f boxPos{VIEWPORT.left * _newSize.x, VIEWPORT.top * _newSize.y};
	const sf::Vector2f boxSize{VIEWPORT.width * _newSize.x, heightWithoutTextEntry * _newSize.y};

	m_infoBox.setPosition(boxPos);
	m_infoBox.setSize(boxSize);

	const float textEntryWidth{m_infoBox.getSize().x};
	const sf::Vector2f textEntryPos{boxPos.x, boxPos.y + boxSize.y};

	m_textEntry.setWidth(textEntryWidth);
	m_textEntry.setPosition(textEntryPos);
}
