#include "ProgramState_Pause.h"

ProgramState_Pause::ProgramState_Pause(Program& _program)
	:ProgramState_Menu(_program, true) {

	addMenuItem("Resume",
				&m_program.popState);

	addMenuItem("Exit",
				&m_program.closeWindow);
}
