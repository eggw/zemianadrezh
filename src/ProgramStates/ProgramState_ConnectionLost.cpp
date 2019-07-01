#include "ProgramStates/ProgramState_ConnectionLost.h"

#include "FontManager.h"

ProgramState_ConnectionLost::ProgramState_ConnectionLost(Program& _program)
            : MenuState(_program.m_window,
						FontManager::get_instance().getFont(FontManager::Type::ANDY)) {
	addMenuItem("Connection to remote server was lost!");
	addGap();
	addMenuItem("Back", std::bind(&Program::returnToMainMenu, &_program), sf::Keyboard::Escape);
}
