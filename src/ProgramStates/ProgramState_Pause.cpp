#include "ProgramStates/ProgramState_Pause.h"

#include "Keybinds.h"

ProgramState_Pause::ProgramState_Pause(Program& _program)
            : ProgramState_Menu(_program, true) {
	addMenuItem("Resume", &Program::popState, sf::Keyboard::Escape);

	addMenuItem("Back to Main Menu", &Program::returnToMainMenu);

	addGap();
	addMenuItem("Exit", &Program::closeWindow);
}
