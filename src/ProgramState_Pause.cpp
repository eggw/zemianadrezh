#include "ProgramState_Pause.h"

#include "Keybinds.h"

ProgramState_Pause::ProgramState_Pause(Program& _program)
	:ProgramState_Menu(_program, true) {

	addMenuItem("Resume",
				&Program::popState);

	addMenuItem("Back to Main Menu",
				&Program::returnToMainMenu);

	addGap();
	addMenuItem("Exit",
				&Program::closeWindow);
}

void ProgramState_Pause::getInput(sf::Event& _event) {
	ProgramState_Menu::getInput(_event);

    if(_event.type == sf::Event::KeyPressed) {
		if(_event.key.code == Key::PAUSE_KEY){
			m_program.popState();
			return;
		}
	}
}
