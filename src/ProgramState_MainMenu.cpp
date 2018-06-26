#include "ProgramState_MainMenu.h"

ProgramState_MainMenu::ProgramState_MainMenu(Program& _program)
	:ProgramState_Menu(_program) {

	addMenuItem("Play",
				&m_program.pushState_Play);

	addMenuItem("Options");

	addMenuItem("Exit",
				&m_program.closeWindow);
}
