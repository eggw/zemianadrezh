#include "ProgramState_MainMenu.h"

#include "ProgramState_Play.h"

ProgramState_MainMenu::ProgramState_MainMenu(Program& _program)
	:ProgramState_Menu(_program) {

	addMenuItem("Single Player",
				&m_program.pushState_Play);

	addMenuItem("Multiplayer");

	addMenuItem("Options");

	addMenuItem("Exit",
				&m_program.closeWindow);
}
