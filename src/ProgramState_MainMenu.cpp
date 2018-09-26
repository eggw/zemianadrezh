#include "ProgramState_MainMenu.h"

ProgramState_MainMenu::ProgramState_MainMenu(Program& _program)
	:ProgramState_Menu(_program) {

	addMenuItem("Singleplayer",
				&Program::pushState_SPPlay);

	addMenuItem("Multiplayer",
				&Program::pushState_MPMenu);

	addMenuItem("Options");

	addGap();
	addMenuItem("Exit",
				&Program::closeWindow);
}
