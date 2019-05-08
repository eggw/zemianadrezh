#ifndef PROGRAMSTATE_CONNECTIONLOST_H_INCLUDED
#define PROGRAMSTATE_CONNECTIONLOST_H_INCLUDED

#include "ProgramStates/ProgramState_Menu.h"

class ProgramState_ConnectionLost : public ProgramState_Menu {
  public:
	ProgramState_ConnectionLost(Program& _program);
};

#endif // PROGRAMSTATE_CONNECTIONLOST_H_INCLUDED
