#include "ProgramState_MPHostMenu.h"

ProgramState_MPHostMenu::ProgramState_MPHostMenu(Program& _program)
	:ProgramState_Menu(_program) {

	addMenuItem("Start",
				&Program::pushState_Play_MP_Host);

	addGap();
	addMenuItem("Back to Main Menu",
				&Program::returnToMainMenu);

}
