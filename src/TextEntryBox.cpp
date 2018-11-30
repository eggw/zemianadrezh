#include "TextEntryBox.h"

#include "Keybinds.h"

#include "FontManager.h"
#include "InputLocker.h"

#include <iostream>

const bool keyPressed(sf::Keyboard::Key _key) {
	return sf::Keyboard::isKeyPressed(_key);
}

const bool keysPressedTogether(std::vector<sf::Keyboard::Key> _keys) {
	bool allPressed{true};
	for(auto& key : _keys) {
		if(!keyPressed(key)) {
			allPressed = false;
			break;
		}
	}
	return allPressed;
}

TextEntryBox::TextEntryBox(unsigned int _charSize,
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
	 m_charSize{_charSize},
	 m_maxChars{m_maxChars} {

	m_rectangle.setFillColor(sf::Color(0,0,0,120));
	m_highlightedRectangle.setFillColor(sf::Color(250,250,250,100));

	m_text.setCharacterSize(m_charSize);
	m_text.setOutlineThickness(1);
	m_caret.setCharacterSize(m_charSize);

	m_text.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));
	m_caret.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));
	m_caret.setFillColor(sf::Color(230,230,230));
	m_caret.setString("|");
}

//TODO: Since the last variable in _viewPort (height) is going to be determined
//by the character size, this function should take 3 floats instead of a FloatRect
void TextEntryBox::initialise(sf::Vector2u _windowSize,
				 sf::FloatRect _viewPort) {

    _viewPort.height =  getHeightAsRatio(_windowSize);
    m_textView.setViewport(_viewPort);

	float newWidth{_windowSize.x * m_textView.getViewport().width};

	m_textView.reset({0,
					  0,
					  newWidth,
					  getHeightAsPixels()
					 });


	sf::Vector2f rectSize{m_textView.getSize()};
	m_rectangle.setSize(rectSize);

	float lineSpacing {getHeightAsPixels()};
	float textHeight{m_text.getPosition().y + lineSpacing};
	float rectangleHeight{m_rectangle.getGlobalBounds().height * 0.8f};

	if(textHeight > rectangleHeight) {
		float difference{textHeight - rectangleHeight};
		sf::Vector2f newTextPosition{m_text.getPosition()};
		newTextPosition.y -= difference;
		m_text.setPosition(newTextPosition);
	}
}

void TextEntryBox::getInput(sf::Event& _event) {
	switch(_event.type) {
	case sf::Event::KeyPressed: {
			if(_event.key.code == Key::CHAT_SEND) {
				if(m_enteringText && !stringEmpty()) {
					m_inputComplete = true;
					m_lastString = m_text.getString();
					if(!m_alwaysActive) {
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


			else if(keysPressedTogether({Key::LSHIFT,
										 Key::TEXT_SELECTLEFT
										})) {
				selectLeft();
			}

			else if(keysPressedTogether({Key::LSHIFT,
										 Key::TEXT_SELECTRIGHT
										})) {
				selectRight();
			}

			else if(keysPressedTogether({Key::LCTRL,
										 Key::TEXT_SELECTALL
										})) {
				selectAll();
			}

			else if(keysPressedTogether({Key::LCTRL,
										 Key::TEXT_COPY
										})) {
				sf::Clipboard::setString(getSelection());
			}

			else if(keysPressedTogether({Key::LCTRL,
										 Key::TEXT_CUT
										})) {
				sf::Clipboard::setString(getSelection());
				if(sequenceSelected()){
					deleteSelection();
				}
			}

			else if(keysPressedTogether({Key::LCTRL,
										 Key::TEXT_PASTE
										})) {
				if(sequenceSelected()){
					deleteSelection();
				}
				std::string s{sf::Clipboard::getString()};
				insert(s);
			}

			else if(keyPressed(Key::TEXT_MOVELEFT)) {
				moveLeft();
			}

			else if(keyPressed(Key::TEXT_MOVERIGHT)) {
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
			if(sequenceSelected() && !keyPressed(Key::LCTRL)) {
				deleteSelection();
			}

			//8 = backspace
			//for some reason, 127 (delete)
			//isn't registering
			if(_event.text.unicode == 8) {
				deleteSelection();
			}

			else if(validInsertion(_event.text.unicode)) {

				if(sequenceSelected()) {
					deleteSelection();
				}

				insert(static_cast<char>(_event.text.unicode));
			}

			break;
		}

	default:
		break;

	}

}

void TextEntryBox::update() {
	if(m_alwaysActive) {
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
	if(m_enteringText && !sequenceSelected()) {
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

std::string TextEntryBox::getCurrentString() const {
	return m_text.getString();
}

void TextEntryBox::setActive() {
	m_enteringText = true;
}

void TextEntryBox::setInactive() {
	m_enteringText = false;
}

void TextEntryBox::setAlwaysVisible(bool _val) {
	m_alwaysVisible = _val;
}

void TextEntryBox::setAlwaysActive(bool _val) {
	m_alwaysActive = _val;
}

float TextEntryBox::getHeightAsPixels() const {
	return (FontManager::get_instance().getLineSpacing(FontManager::Type::ANDY, m_charSize));
}

float TextEntryBox::getHeightAsRatio(sf::Vector2u _windowSize) const {
	return 1 - (_windowSize.y - getHeightAsPixels()) /_windowSize.y;
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

		sf::Color caretColor = m_caret.getFillColor();
		caretColor.a = caretAlphaValue;
		m_caret.setFillColor(caretColor);

		//float caret_x{m_text.getGlobalBounds().width};
		//float caret_x{float(m_currentStringIndexPosition * m_charSize)};


		m_caret.setPosition({textXAtPosition(m_selectionBegin), -2});
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

void TextEntryBox::updateHighlight() {
	if(sequenceSelected()) {
		float startPos{textXAtPosition(m_selectionBegin)};
		float endPos{textXAtPosition(m_selectionEnd)};

		m_highlightedRectangle.setSize({endPos - startPos,
										m_rectangle.getGlobalBounds().height
									   });

		m_highlightedRectangle.setPosition(startPos,
										   m_text.getPosition().y);
	}
	else {
		m_highlightedRectangle.setSize({});
	}
}

bool TextEntryBox::stringEmpty() const {
	return m_text.getString().isEmpty();
}

void TextEntryBox::selectAll() {
	m_selectionBegin = 0;
	m_selectionEnd = m_text.getString().getSize();
	m_selectionDirection = 0;
}

void TextEntryBox::unselectAll() {
	m_selectionEnd = m_selectionBegin;
	m_selectionDirection = 0;
}

void TextEntryBox::selectLeft() {
	if(keyPressed(Key::LCTRL)){
		m_selectionBegin = posAtPreviousWord();
	}
	else if(m_selectionDirection <= 0) {
		if(m_selectionBegin <= 0) {
			return;
		}
		--m_selectionBegin;
	}
	else {
		--m_selectionEnd;
	}
	--m_selectionDirection;
}

void TextEntryBox::selectRight() {
	if(keyPressed(Key::LCTRL)){
		m_selectionEnd = posAtNextWord();
	}
	else if(m_selectionDirection >= 0) {
		if(m_selectionEnd >= m_text.getString().getSize()) {
			return;
		}
		++m_selectionEnd;
	}
	else {
		++m_selectionBegin;
	}
	++m_selectionDirection;
}

void TextEntryBox::moveLeft() {
	if(keyPressed(Key::LCTRL)){
		m_selectionBegin = posAtPreviousWord();
		m_selectionEnd = m_selectionBegin;
	}
	else if(!sequenceSelected() && m_selectionBegin > 0) {
		--m_selectionBegin;
		m_selectionEnd = m_selectionBegin;
	}
	else if(sequenceSelected()) {
		m_selectionEnd = m_selectionBegin;
	}
	m_selectionDirection = 0;
}

void TextEntryBox::moveRight() {
	if(keyPressed(Key::LCTRL)){
		m_selectionBegin = posAtNextWord();
		m_selectionEnd = m_selectionBegin;
	}
	else if(!sequenceSelected() && m_selectionBegin < m_text.getString().getSize()) {
		++m_selectionBegin;
		m_selectionEnd = m_selectionBegin;
	}
	else if(sequenceSelected()) {
		m_selectionBegin = m_selectionEnd;
	}
	m_selectionDirection = 0;
}

size_t TextEntryBox::posAtPreviousWord() {
	size_t pos{m_selectionBegin};
	if(pos == 0){
		return pos;
	}

	std::string s{m_text.getString()};
	std::string previousChar{s.substr(pos-1, 1)};
	if(previousChar == " "){
		while(pos > 0 && s.substr(pos-1, 1) == " "){
			--pos;
		}
	}

	while(pos > 0 && s.substr(pos-1, 1) != " "){
		--pos;
	}

	return pos;
}

size_t TextEntryBox::posAtNextWord() {
	size_t pos{m_selectionEnd};
	std::string s{m_text.getString()};
	if(pos == s.length()){
		return pos;
	}

	std::string previousChar{s.substr(pos, 1)};
	if(previousChar == " "){
		while(pos < s.length() && s.substr(pos, 1) == " "){
			++pos;
		}
	}

	while(pos < s.length() && s.substr(pos, 1) != " "){
		++pos;
	}

	return pos;
}

bool TextEntryBox::sequenceSelected() const {
	return (m_selectionBegin != m_selectionEnd);
}

void TextEntryBox::deleteSelection() {
	if(stringEmpty()){
		return;
	}

	std::string newString{m_text.getString()};

	if(sequenceSelected()) {
		newString.erase(m_selectionBegin,
						m_selectionEnd - m_selectionBegin);
	}
	else {
		selectLeft();
		newString.erase(m_selectionBegin, 1);

	}

	m_selectionEnd = m_selectionBegin;
	m_selectionDirection = 0;
	m_text.setString(newString);
}

const std::string TextEntryBox::getSelection() const {
    std::string s{m_text.getString()};
    return s.substr(m_selectionBegin, m_selectionEnd);
}

bool TextEntryBox::validInsertion(sf::Uint32 _unicode) {
	//Anything between 32 and 255
	//represents the rest of the
	//alphanumeric characters, and
	//some symbols
	if(_unicode >= 32
		&&
		_unicode <= 255
		&&
		m_text.getString().getSize() < m_maxChars) {
		return true;
	}

	return false;
}

void TextEntryBox::insert(std::string& _str) {
	size_t currentLength{m_text.getString().getSize()};
	size_t combinedLength{_str.length() + currentLength};
	if(combinedLength > m_maxChars) {
        _str = _str.substr(0, m_maxChars - currentLength);
	}

	std::string newString{m_text.getString()};
	newString.insert(m_selectionBegin,
					 _str);

	m_text.setString(newString);

	m_selectionEnd = m_selectionBegin + _str.length();
	m_selectionBegin = m_selectionEnd;
	m_selectionDirection = 0;
}

void TextEntryBox::insert(char _char) {
	std::string newString{m_text.getString()};
	newString.insert(m_selectionBegin,
					 1,
					 _char);

	if(newString.length() > m_maxChars){
        newString = newString.substr(0, m_maxChars);
    }

	m_text.setString(newString);
	moveRight();
}

void TextEntryBox::clearText() {
	m_selectionBegin = 0;
	m_selectionEnd = 0;
	m_selectionDirection = 0;
	m_text.setString("");
	updateCaret();
}

float TextEntryBox::textXAtPosition(size_t _index) {
	if(stringEmpty()) {
		return 0;
	}

	float averageCharWidth{ m_text.getGlobalBounds().width /
							m_text.getString().getSize()};

	return {float(_index
				  * averageCharWidth)};
}
