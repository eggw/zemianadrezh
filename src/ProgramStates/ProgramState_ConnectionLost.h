#ifndef PROGRAMSTATE_CONNECTIONLOST_H_INCLUDED
#define PROGRAMSTATE_CONNECTIONLOST_H_INCLUDED

#include <SPSS/System/MenuState.h>

#include "Program.h"

class ProgramState_ConnectionLost : public spss::MenuState {
  public:
	ProgramState_ConnectionLost(Program& _program);
};

#endif // PROGRAMSTATE_CONNECTIONLOST_H_INCLUDED
