#include "ProgramState_MainMenu.h"

ProgramState_MainMenu::ProgramState_MainMenu(Program& _program)
    :ProgramState_Menu(_program) {

	addTextItem("Singleplayer");
	addTextItem("Multiplayer");
	addTextItem("Exit");
}
