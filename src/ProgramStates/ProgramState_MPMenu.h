#ifndef PROGRAMSTATE_MPMENU_H_INCLUDED
#define PROGRAMSTATE_MPMENU_H_INCLUDED

#include <SPSS/System/MenuState.h>

#include "Program.h"

class ProgramState_MPMenu : public spss::MenuState {
  public:
	ProgramState_MPMenu(Program& _program);
};

#endif // PROGRAMSTATE_MPMENU_H_INCLUDED
