#include "ProgramState_MPMenu.h"

ProgramState_MPMenu::ProgramState_MPMenu(Program& _program)
	:ProgramState_Menu(_program) {

	addMenuItem("Host");

	addMenuItem("Join");

	addMenuItem("Back",
				&Program::popState);

}
