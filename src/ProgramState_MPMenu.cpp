#include "ProgramState_MPMenu.h"

ProgramState_MPMenu::ProgramState_MPMenu(Program& _program)
	:ProgramState_Menu(_program) {

	addMenuItem("Host");

	addMenuItem("Join");

	addGap();
	addMenuItem("Back",
				&Program::popState);

}
