#include "ProgramStates/ProgramState_MainMenu.h"

#include "FontManager.h"

ProgramState_MainMenu::ProgramState_MainMenu(Program& _program)
            : MenuState(_program.m_window,
                        FontManager::get_instance().getFont(FontManager::TYPE::ANDY),
                        "zemianadrezh") {
	setRandomisedColors(true);
	setScalingText(true);
	setRotatingText(true);

	addMenuItem("Singleplayer", std::bind(&Program::pushState_Play_SP, &_program));
	addMenuItem("Multiplayer", std::bind(&Program::pushState_MPMenu, &_program));
	addGap();
	addMenuItem("Exit", std::bind(&Program::exit, &_program));
}

void ProgramState_MainMenu::onStateSwitch() {
	MenuState::onStateSwitch();
}
