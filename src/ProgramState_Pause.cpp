#include "ProgramState_Pause.h"

ProgramState_Pause::ProgramState_Pause(Program& _program)
	:ProgramState_Menu(_program, true) {

	addMenuItem("Resume",
				&Program::popState);

	addMenuItem("Exit",
				&Program::closeWindow);
}
