#include "ProgramState_MainMenu.h"

ProgramState_MainMenu::ProgramState_MainMenu(Program& _program)
	:ProgramState_Menu(_program) {

	addMenuItem("Play",
				&Program::pushState_Play);

	addMenuItem("Options");

	addMenuItem("Exit",
				&Program::closeWindow);
}
