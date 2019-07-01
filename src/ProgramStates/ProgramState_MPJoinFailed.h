#ifndef PROGRAMSTATE_MPJOINFAILED_H_INCLUDED
#define PROGRAMSTATE_MPJOINFAILED_H_INCLUDED

#include <SPSS/System/MenuState.h>

#include "Program.h"

class ProgramState_MPJoinFailed : public spss::MenuState {
  public:
	ProgramState_MPJoinFailed(Program& _program, const std::string& _ip);
};

#endif // PROGRAMSTATE_MPJOINFAILED_H_INCLUDED
