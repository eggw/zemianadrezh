#ifndef PROGRAMSTATE_MAINMENU_H_INCLUDED
#define PROGRAMSTATE_MAINMENU_H_INCLUDED

#include <SPSS/System/MenuState.h>

#include "Program.h"

class ProgramState_MainMenu : public spss::MenuState {
  public:
	ProgramState_MainMenu(Program& _program);
	void onStateSwitch();
};

#endif // PROGRAMSTATE_MAINMENU_H_INCLUDED
