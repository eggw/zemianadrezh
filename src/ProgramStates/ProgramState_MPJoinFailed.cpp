#include "ProgramStates/ProgramState_MPJoinFailed.h"

ProgramState_MPJoinFailed::ProgramState_MPJoinFailed(Program& _program,
                                                     const std::string& _ip)
            : ProgramState_Menu(_program) {
	addMenuItem("Could not connect to " + _ip);
	addGap();
	addMenuItem("Back", &Program::popState, sf::Keyboard::Escape);
}
