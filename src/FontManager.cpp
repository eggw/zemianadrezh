#include "FontManager.h"

FontManager::FontManager() {
	sf::Font andy;

	andy.loadFromFile("assets/fonts/andy.ttf");
	m_fonts.emplace(FontManager::TYPE::ANDY, andy);
}

const sf::Font& FontManager::getFont(FontManager::TYPE _type) {
	return m_fonts[_type];
}

float FontManager::getLineSpacing(FontManager::TYPE _type, unsigned int _characterSize) {
	return m_fonts[_type].getLineSpacing(_characterSize);
}
