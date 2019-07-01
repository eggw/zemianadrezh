#ifndef PROGRAMSTATE_MPHOSTMENU_H_INCLUDED
#define PROGRAMSTATE_MPHOSTMENU_H_INCLUDED

#include <SPSS/System/MenuState.h>

#include "Program.h"

class ProgramState_MPHostMenu : public spss::MenuState {
  public:
	ProgramState_MPHostMenu(Program& _program);
};

#endif // PROGRAMSTATE_MPHOSTMENU_H_INCLUDED
