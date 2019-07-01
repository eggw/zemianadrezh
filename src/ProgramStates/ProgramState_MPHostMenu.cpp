#include "ProgramStates/ProgramState_MPHostMenu.h"

#include "FontManager.h"

ProgramState_MPHostMenu::ProgramState_MPHostMenu(Program& _program)
            : MenuState(_program.m_window,
						FontManager::get_instance().getFont(FontManager::Type::ANDY),
						"Multiplayer - Host") {
	addMenuItem("Start", std::bind(&Program::pushState_Play_MP_Host, &_program));

	addGap();
	addMenuItem("Back to Main Menu", std::bind(&Program::returnToMainMenu, &_program));
}
