#ifndef PROGRAMSTATE_PAUSE_H_INCLUDED
#define PROGRAMSTATE_PAUSE_H_INCLUDED

#include <SPSS/System/MenuState.h>

#include "Program.h"

class ProgramState_Pause : public spss::MenuState {
  public:
	ProgramState_Pause(Program& _program);
};

#endif // PROGRAMSTATE_PAUSE_H_INCLUDED
