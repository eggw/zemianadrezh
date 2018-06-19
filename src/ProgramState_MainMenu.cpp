#include "ProgramState_MainMenu.h"

ProgramState_MainMenu::ProgramState_MainMenu(Program& _program)
    :ProgramState_Menu(_program) {

	addTextItem("Singleplayer");
	addTextItem("Multiplayer");

}

ProgramState_MainMenu::~ProgramState_MainMenu() {

}

void ProgramState_MainMenu::getInput() {

}

void ProgramState_MainMenu::update() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(*m_program.m_window);

}

void ProgramState_MainMenu::draw() {
	for(auto& text : m_texts){
		m_program.m_window->draw(text);
	}
}
