#include "ProgramState_MPHostMenu.h"

ProgramState_MPHostMenu::ProgramState_MPHostMenu(Program& _program)
	:ProgramState_Menu(_program) {

	addMenuItem("Start");

	addGap();
	addMenuItem("Back to Main Menu",
				&Program::returnToMainMenu);

}
