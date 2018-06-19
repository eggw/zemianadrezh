#include "ProgramState_Menu.h"

#include "FontManager.h"

ProgramState_Menu::ProgramState_Menu(Program& _program)
    :ProgramState(_program) {

}

ProgramState_Menu::~ProgramState_Menu() {

}

void ProgramState_Menu::getInput() {

}

void ProgramState_Menu::update() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(*m_program.m_window);

}

void ProgramState_Menu::draw() {
	for(auto& text : m_texts){
		m_program.m_window->draw(text);
	}
}

void ProgramState_Menu::addTextItem(const std::string _string){
	sf::Text text;
	float verticalSpacing{40.0f};

	text.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));
	text.setString(_string);
	text.setOrigin(	text.getGlobalBounds().width / 2,
					text.getGlobalBounds().height / 2);

	m_texts.push_back(text);

	int index = floor(m_texts.size() / 2);

    std::vector<sf::Text>::iterator it = m_texts.begin();
    it += index;

    //Set the element in the center to be in the middle of the
    //screen
	it->setPosition(m_program.m_window->getSize().x / 2,
					m_program.m_window->getSize().y / 2);

    //Then, set the position all elements above that one
    while(it != m_texts.begin()){
        sf::Vector2f lastPosition = it->getPosition();
        it--;
        it->setPosition(lastPosition.x, lastPosition.y - verticalSpacing);
    }

    it = m_texts.begin();
    it += index;

	//Finally, set the positions for all elements above the center
    while(it != m_texts.end() - 1){
        sf::Vector2f lastPosition = it->getPosition();
        it++;
        it->setPosition(lastPosition.x, lastPosition.y + verticalSpacing);
    }

}
