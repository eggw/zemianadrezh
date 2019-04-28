#include "ChatBoxMessage.h"

void removeNewlines(std::string& _str) {
	std::string newline{"\n"};
	auto        i = _str.find(newline);
	while (i != std::string::npos) {
		_str.erase(i, newline.length());
		i = _str.find(newline, i);
	}
}

ChatBoxMessage::ChatBoxMessage(const std::string& _sender,
                               const std::string& _message,
                               const sf::Font&    _font,
                               unsigned int       _charSize)
            : m_text{}, m_sender{_sender}, m_message{_message} {
	m_text.setFont(_font);
	m_text.setCharacterSize(_charSize);

	std::string finalStr{_message};
	if (_sender != "") {
		finalStr = ("<" + _sender + "> " + finalStr);
	}

	setString(finalStr);
}

void ChatBoxMessage::draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const {
	target.draw(m_text, states);
}

void ChatBoxMessage::fitWidth(float _width) {
	bool tooWide{m_text.getGlobalBounds().width >= _width};
	bool tooNarrow{m_text.getGlobalBounds().width < _width &&
	               getNumberOfLines() > 1};

	if (!tooWide && !tooNarrow) {
		return;
	}

	std::string textStr = m_text.getString();
	removeNewlines(textStr);
	setString(textStr);

	float charSize = m_text.getGlobalBounds().width / textStr.size();
	float currentWidth{0};

	for (size_t i{0}; i < textStr.length(); i++) {
		currentWidth += charSize;

		//Insert a newline at the position, if it's
		//too wide
		if (currentWidth >= _width) {
			textStr.insert(i, 1, '\n');
			currentWidth = 0;
		}

		//If we have a space as the first character
		//on a new line, we'll get rid of it
		else if (currentWidth == charSize) {
			if (isspace(textStr.at(i))) {
				textStr.erase(i, 1);
			}
		}
	}

	setString(textStr);
}

void ChatBoxMessage::setString(const std::string& _str) {
	m_text.setString(_str);

	//After changing the string, our fill/outline colours
	//and outline thicknesses are reset, so we'll reapply them

	m_text.setOutlineThickness(1);

	//If m_sender contains a newline, we'll want to make sure that
	//the whole name gets its colour set. Therefore, we'll check how
	//many newlines there are in the name part and make sure to add
	//the result to the end position of setFillColor().
	//Since getNumberOfLines returns 1 normally, we'll decrement our
	//initial result.
	unsigned int endOffset{getNumberOfLines(1, m_sender.length())};
	--endOffset;

	if (m_sender == "Server") {
		m_text.setFillColor(sf::Color::Red, 1, m_sender.length() + endOffset);
	}
	else if (m_sender == "LocalPlayer") {
		m_text.setFillColor(sf::Color::Cyan, 1, m_sender.length() + endOffset);
	}
	else {
		m_text.setFillColor(
		  sf::Color::Magenta, 1, m_sender.length() + endOffset);
	}
}

void ChatBoxMessage::setPosition(sf::Vector2f _pos) {
	m_text.setPosition(_pos);
}

void ChatBoxMessage::setTransparency(int _a) {
	m_text.setTransparency(_a);
}

sf::Vector2f ChatBoxMessage::getPosition() const {
	return m_text.getPosition();
}

int ChatBoxMessage::getTransparency() const {
	return m_text.getFillColor(0).a;
}

unsigned int ChatBoxMessage::getNumberOfLines(size_t _startPos,
                                              size_t _endPos) const {
	const std::string& s{m_text.getString()};
	if (_endPos == 0) {
		_endPos = s.length() - 1;
	}
	std::string sub{s.substr(_startPos, _endPos)};

	int    count{1};
	size_t nPos = sub.find("\n");
	while (nPos != std::string::npos) {
		count++;
		nPos = sub.find("\n", nPos + 1);
	}

	return count;
}
