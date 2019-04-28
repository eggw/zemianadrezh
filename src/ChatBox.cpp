#include "ChatBox.h"

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

ChatBox::ChatBox(sf::RenderWindow& _window, const std::string& _name)
            : m_name(_name),
              m_view(),
              m_shadedRectangleView(),
              m_shadedRectangle(),
              m_messages(),
              m_textEntry(),
              m_clock(),
              m_anchoredToBottom(true) {
	onResize(_window.getSize());

	/*
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
	*/

	m_shadedRectangle.setOutlineColor(sf::Color(255, 165, 0));
	snapToBottom();
}

void ChatBox::appendMessage(const std::string _message,
                            const std::string _sender) {
	ChatBoxMessage newMessage{
	  _sender,
	  _message,
	  FontManager::get_instance().getFont(FontManager::Type::ANDY),
	  CHARACTER_SIZE};

	newMessage.fitWidth(m_view.getSize().x * 0.9f);

	m_messages.push_back(newMessage);
	positionMessage(m_messages.size() - 1);
	m_clock.restart();

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
			m_clock.restart();
			setTransparency(255);
		}

		else if (_event.key.code == Key::CHAT_UP) {
			m_anchoredToBottom = false;
			scrollUp();
		}

		else if (_event.key.code == Key::CHAT_DOWN) {
			m_anchoredToBottom = false;
			scrollDown();
		}

		else if (_event.key.code == Key::CHAT_TOP) {
			m_anchoredToBottom = false;
			snapToTop();
		}

		else if (_event.key.code == Key::CHAT_BOTTOM) {
			snapToBottom();
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

void ChatBox::update() {
	m_textEntry.update();
	if (m_textEntry.inputComplete()) {
		m_lastMessage = std::make_pair(m_textEntry.getLastString(), m_name);
	}
	if (m_textEntry.enteringText()) {
		m_clock.restart();
	}
	updateShadedRectangleTransparency();
	updateMessageTransparency();
	updateMessageAlertTransparency();
}

void ChatBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::View previousView = target.getView();

	target.setView(m_shadedRectangleView);
	target.draw(m_shadedRectangle, states);

	target.setView(m_view);
	if (!messagesTransparent()) {
		for (const auto& message : m_messages) {
			target.draw(message, states);
		}
	}

	target.draw(m_textEntry, states);

	target.setView(previousView);
}

bool ChatBox::completedMessage(std::pair<std::string, std::string>* _ptr) {
	if (m_lastMessage.first == "" && m_lastMessage.second == "") {
		return false;
	}

	*_ptr                = m_lastMessage;
	m_lastMessage.first  = "";
	m_lastMessage.second = "";

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
	m_clock.restart();
	setTransparency(255);

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
	m_clock.restart();
	setTransparency(255);
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
	m_clock.restart();
	setTransparency(255);

	if (!viewAtHighest()) {
		m_view.setCenter(m_view.getCenter().x,
		                 m_view.getCenter().y - (LINESPACING));
	}
	if (viewAtHighest()) {
		snapToTop();
	}
}

void ChatBox::scrollDown() {
	m_clock.restart();
	setTransparency(255);

	if (!viewAtLowest()) {
		m_view.setCenter(m_view.getCenter().x,
		                 m_view.getCenter().y + (LINESPACING));
	}
	if (viewAtLowest()) {
		snapToBottom();
	}
}
