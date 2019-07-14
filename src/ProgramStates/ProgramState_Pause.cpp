#include "ProgramStates/ProgramState_Pause.h"

#include "FontManager.h"
#include "Keybinds.h"

ProgramState_Pause::ProgramState_Pause(Program& _program)
            : MenuState(_program.m_window,
                        FontManager::get_instance().getFont(FontManager::Type::ANDY),
                        "Paused") {
	addMenuItem("Resume", std::bind(&Program::popState, &_program), sf::Keyboard::Escape);

	addMenuItem("Back to Main Menu", std::bind(&Program::returnToMainMenu, &_program));

	addGap();
	addMenuItem("Exit", std::bind(&Program::exit, &_program));
}
