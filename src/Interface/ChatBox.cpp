#include "Interface/ChatBox.h"

#include "FontManager.h"

#include "Keybinds.h"

//Here are the variables we'll use to initialise the text entry box
//in the correct position
//Note that the height (0.25 by default) specifies that of both the
//ChatBox and m_textEntry combined.
//////////////////////////////////////////////////////////////////////////////////////////////
const sf::FloatRect    VIEWPORT{0, 0.75f, 0.3f, 0.25f};
constexpr float        SECONDS_UNTIL_MESSAGES_FADE{5.0f};
constexpr unsigned int CHARACTER_SIZE{20};

const float LINESPACING =
  FontManager::get_instance().getLineSpacing(FontManager::Type::ANDY,
                                             CHARACTER_SIZE);
const float Y_BUFFERSPACE{1.3f * LINESPACING};

//The value of 1.3 above is because the origin of the text's position is top
//left, in order to get a little aesthetic extra buffer space below the
//last message
//////////////////////////////////////////////////////////////////////////////////////////////

ChatBox::ChatBox(const std::string& _name)
            : m_target{nullptr},
              m_lastTargetSize{},
              m_name{_name},
              m_view{},
              m_shadedRectangleView{},
              m_shadedRectangle{},
              m_messages{},
              m_lastMessage{},
              m_textEntry{},
              m_clock{},
              m_anchoredToBottom{true} {
	m_shadedRectangle.setOutlineColor(sf::Color(255, 165, 0));
	snapToBottom();
}

void ChatBox::appendMessage(const Message _msg) {
	ChatBoxMessage newMessage{
	  _msg,
	  FontManager::get_instance().getFont(FontManager::Type::ANDY),
	  CHARACTER_SIZE};

	newMessage.fitWidth(m_view.getSize().x * 0.9f);

	m_messages.push_back(newMessage);
	positionMessage(m_messages.size() - 1);
	resetTransparency();

	if (m_anchoredToBottom) {
		snapToBottom();
	}

	//If we get a new message and we're not anchored to the bottom, we make sure to
	//enable the visual notification of a new unviewed message
	else {
		setNewMessageAlert(true);
	}
}

void ChatBox::getInput(sf::Event& _event) {
	//In the case of the enter key, we're going to poll an event.
	//The reason for this is that we only want to toggle this once,
	//and not have it rapidly be called every frame.
	m_textEntry.getInput(_event);

	switch (_event.type) {
	case sf::Event::KeyPressed: {
		if (_event.key.code == Key::CHAT_SEND) {
			resetTransparency();
		}

		else if (_event.key.code == Key::CHAT_UP) {
			scrollUp();
		}

		else if (_event.key.code == Key::CHAT_DOWN) {
			scrollDown();
		}

		else if (_event.key.code == Key::CHAT_TOP) {
			snapToTop();
		}

		else if (_event.key.code == Key::CHAT_BOTTOM) {
			snapToBottom();
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

	m_textEntry.update();
	if (m_textEntry.inputComplete()) {
		m_lastMessage = {m_name, m_textEntry.getLastString()};
	}
	if (m_textEntry.enteringText()) {
		resetTransparency();
	}
	updateShadedRectangleTransparency();
	updateMessageTransparency();
	updateMessageAlertTransparency();
}

void ChatBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	m_target = &target;

	sf::View previousView = m_target->getView();
	m_target->setView(m_shadedRectangleView);
	m_target->draw(m_shadedRectangle, states);

	m_target->setView(m_view);
	if (!messagesTransparent()) {
		for (const auto& message : m_messages) {
			m_target->draw(message, states);
		}
	}

	m_target->draw(m_textEntry, states);

	m_target->setView(previousView);
}

bool ChatBox::completedMessage(Message* _ptr) {
	if (m_lastMessage.sender == "" && m_lastMessage.content == "") {
		return false;
	}

	*_ptr                 = m_lastMessage;
	m_lastMessage.sender  = "";
	m_lastMessage.content = "";

	return true;
}

//This function sets the position of a new message, at the
//very bottom of the box
void ChatBox::positionMessage(int _index) {
	if (!m_messages.empty() && _index > 0) {
		ChatBoxMessage& message = m_messages[_index];
		sf::Vector2f    newPosition{m_messages[_index - 1].getPosition()};
		unsigned int    lastMessageLines{
          m_messages[_index - 1].getNumberOfLines()};
		newPosition.y += lastMessageLines * (LINESPACING);
		message.setPosition(newPosition);
	}
}

void ChatBox::setTransparency(int _a) {
	for (auto& message : m_messages) {
		message.setTransparency(_a);
	}
}

bool ChatBox::messagesTransparent() const {
	//Since all messages will have the same transparency,
	//it's enough for us to get the alpha value of the first
	//element
	if (!m_messages.empty()) {
		return m_messages.front().getTransparency() == 0;
	}
	return true;
}

void ChatBox::updateShadedRectangleTransparency() {
	static int rectangleAlphaValue{0};

	if (m_textEntry.enteringText()) {
		rectangleAlphaValue = 25;
	}
	else {
		if (rectangleAlphaValue > 0) {
			rectangleAlphaValue -= 1;
		}
	}

	m_shadedRectangle.setFillColor(sf::Color(0, 0, 0, rectangleAlphaValue));
}

void ChatBox::updateMessageTransparency() {
	static int textAlphaValue{255};

	if (m_clock.getElapsedTime().asSeconds() > SECONDS_UNTIL_MESSAGES_FADE) {
		if (textAlphaValue > 0) {
			textAlphaValue -= 1;
		}

		setTransparency(textAlphaValue);
	}
	else {
		textAlphaValue = 255;
	}
}

void ChatBox::updateMessageAlertTransparency() {
	static bool alphaDecreasing{true};
	static int  alphaValue{255};

	if (alphaDecreasing) {
		alphaValue -= 5;
		if (alphaValue <= 0) {
			alphaDecreasing = false;
		}
	}
	else {
		alphaValue += 5;
		if (alphaValue >= 255) {
			alphaDecreasing = true;
		}
	}

	sf::Color color = m_shadedRectangle.getOutlineColor();
	color.a         = alphaValue;
	m_shadedRectangle.setOutlineColor(color);
}

void ChatBox::setNewMessageAlert(bool _b) {
	if (_b) {
		m_shadedRectangle.setOutlineThickness(-1);
	}
	else {
		m_shadedRectangle.setOutlineThickness(0);
	}
}

void ChatBox::resetTransparency() {
	m_clock.restart();
	setTransparency(255);
}

void ChatBox::onResize(sf::Vector2u _newSize) {
	sf::FloatRect textEntryViewport{
	  0,
	  1 - m_textEntry.getHeightAsRatio(_newSize),
	  VIEWPORT.width,
	  0 //this can be removed as it's unused now
	};

	m_textEntry.initialise(_newSize, textEntryViewport);

	float newViewportHeight{VIEWPORT.height -
	                        (m_textEntry.getHeightAsRatio(_newSize))};

	//I noticed that sometimes, maybe due to float precision reasons, the text entry box
	//and the shaded rectangle overlap. Because I can't reduce the size of the shaded
	//rectangle pixel by pixel easily, I tried decrementing it by 0.0001% until they no
	//longer overlap and there is no visible discrepancy. My do...while loop tests showed
	//that it just needed to be called once to fix this weird visual quirk.
	newViewportHeight -= 0.0001f;

	sf::FloatRect viewRect(
	  {0, 0, _newSize.x * VIEWPORT.width, _newSize.y * newViewportHeight});

	m_view.reset(viewRect);
	m_view.setViewport(
	  {VIEWPORT.left, VIEWPORT.top, VIEWPORT.width, newViewportHeight});

	m_shadedRectangleView.reset(viewRect);
	m_shadedRectangleView.setViewport(m_view.getViewport());

	m_shadedRectangle.setSize(m_shadedRectangleView.getSize());

	for (size_t i{0}; i < m_messages.size(); i++) {
		ChatBoxMessage& message = m_messages[i];
		message.fitWidth(m_view.getSize().x * 0.9f);
		positionMessage(i);
	}

	snapToBottom();
}

//This function checks if the last message is "outside" (below) the view.
//If so, it adjusts the view's center so that the very first message is
//on top.
void ChatBox::snapToTop() {
	if (!viewAtLowest()) {
		m_anchoredToBottom = false;
	}

	resetTransparency();

	if (!m_messages.empty()) {
		ChatBoxMessage& latestMessage = m_messages.back();

		float lastLineYPosition{latestMessage.getPosition().y +
		                        (latestMessage.getNumberOfLines() - 1) *
		                          LINESPACING};

		float boundary{m_view.getSize().y - Y_BUFFERSPACE};

		if (lastLineYPosition < boundary) {
			return;
		}

		ChatBoxMessage& earliestMessage = m_messages.front();
		sf::Vector2f    newCenter{m_view.getCenter().x,
                               earliestMessage.getPosition().y};

		newCenter.y += (m_view.getSize().y / 2);

		m_view.setCenter(newCenter);
	}
}

//This function checks if the last message is "outside" (below) the view.
//If so, it adjusts the view's center so that it is visible.
void ChatBox::snapToBottom() {
	resetTransparency();
	m_anchoredToBottom = true;

	if (!m_messages.empty()) {
		ChatBoxMessage& latestMessage = m_messages.back();
		float           lastLineYPosition{latestMessage.getPosition().y +
                                (latestMessage.getNumberOfLines() - 1) *
                                  LINESPACING};

		float boundary{m_view.getSize().y - Y_BUFFERSPACE};

		if (lastLineYPosition < boundary) {
			return;
		}

		sf::Vector2f newCenter{m_view.getCenter().x, lastLineYPosition};

		newCenter.y -= (m_view.getSize().y / 2) - (Y_BUFFERSPACE);

		m_view.setCenter(newCenter);
	}

	//We can safely assume that all new messages are "viewed" after having
	//snapped to the bottom; therefore, we call this.
	setNewMessageAlert(false);
}

float ChatBox::getUpperViewBound() const {
	return 0;
}
float ChatBox::getLowerViewBound() const {
	if (m_messages.empty()) {
		return 0;
	}
	float lowestPoint{m_messages.back().getPosition().y};
	lowestPoint += Y_BUFFERSPACE;
	return lowestPoint;
}

bool ChatBox::viewAtHighest() const {
	if (m_view.getCenter().y - m_view.getSize().y / 2 <= getUpperViewBound()) {
		return true;
	}

	return false;
}

bool ChatBox::viewAtLowest() const {
	if (m_view.getCenter().y + m_view.getSize().y / 2 >= getLowerViewBound()) {
		return true;
	}

	return false;
}

void ChatBox::scrollUp() {
	if (!viewAtLowest()) {
		m_anchoredToBottom = false;
	}

	resetTransparency();

	if (!viewAtHighest()) {
		m_view.setCenter(m_view.getCenter().x,
		                 m_view.getCenter().y - (LINESPACING));
	}
	if (viewAtHighest()) {
		snapToTop();
	}
}

void ChatBox::scrollDown() {
	if (!viewAtLowest()) {
		m_anchoredToBottom = false;
	}

	resetTransparency();

	if (!viewAtLowest()) {
		m_view.setCenter(m_view.getCenter().x,
		                 m_view.getCenter().y + (LINESPACING));
	}
	if (viewAtLowest()) {
		snapToBottom();
	}
}
