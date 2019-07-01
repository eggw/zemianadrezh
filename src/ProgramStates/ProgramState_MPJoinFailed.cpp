#include "ProgramStates/ProgramState_MPJoinFailed.h"

#include "FontManager.h"

ProgramState_MPJoinFailed::ProgramState_MPJoinFailed(Program& _program,
                                                     const std::string& _ip)
            : MenuState(_program.m_window,
						FontManager::get_instance().getFont(FontManager::Type::ANDY)) {
	addMenuItem("Could not connect to " + _ip);
	addGap();
	addMenuItem("Back", std::bind(&Program::popState, &_program), sf::Keyboard::Escape);
}
