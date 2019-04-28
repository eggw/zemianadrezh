#include "ProgramState_ConnectionLost.h"

ProgramState_ConnectionLost::ProgramState_ConnectionLost(Program& _program)
            : ProgramState_Menu(_program) {
	addMenuItem("Connection to remote server was lost!");
	addGap();
	addMenuItem("Back", &Program::returnToMainMenu, sf::Keyboard::Escape);
}
