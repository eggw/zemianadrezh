#ifndef PROGRAMSTATE_MPJOINFAILED_H_INCLUDED
#define PROGRAMSTATE_MPJOINFAILED_H_INCLUDED

#include "ProgramState_Menu.h"

class ProgramState_MPJoinFailed : public ProgramState_Menu {
	public:
		ProgramState_MPJoinFailed (Program& _program, const std::string& _ip);
};

#endif // PROGRAMSTATE_MPJOINFAILED_H_INCLUDED
