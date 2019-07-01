#include "ProgramStates/ProgramState_MPMenu.h"

#include "FontManager.h"

ProgramState_MPMenu::ProgramState_MPMenu(Program& _program)
            : MenuState(_program.m_window,
						FontManager::get_instance().getFont(FontManager::Type::ANDY),
						"Multiplayer") {
	addMenuItem("Host", std::bind(&Program::pushState_MPHostMenu, &_program));

	addMenuItem("Join", std::bind(&Program::pushState_MPJoinMenu, &_program));

	addGap();
	addMenuItem("Back", std::bind(&Program::popState, &_program));
}
