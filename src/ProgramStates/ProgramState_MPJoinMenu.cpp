#include "ProgramStates/ProgramState_MPJoinMenu.h"

#include "FontManager.h"

ProgramState_MPJoinMenu::ProgramState_MPJoinMenu(Program& _program)
            : MenuState(_program.m_window,
                        *FontManager::get_instance().get(FONT::ANDY),
                        "Multiplayer - Join"),
              m_IPPrompt{true,
						{m_window.getSize().x / 2.F,
						 m_window.getSize().y / 2.5F},
						*FontManager::get_instance().get(FONT::ANDY),
						"IP Address",
						28,
						"localhost",
						24} {
	m_IPStr = std::make_unique<std::string>("");
	m_IPPrompt.setDraggable(false);
	m_IPPrompt.setMaxChars(45);
	m_IPPrompt.addButton("Connect", std::bind(&Program::pushState_Play_MP_Join, &_program, m_IPStr.get()));

	addMenuItem("Back to Main Menu", {0.5F, 0.75F}, std::bind(&Program::returnToMainMenu, &_program));

	onResize(_program.m_window.getSize());
}

void ProgramState_MPJoinMenu::getInput(sf::Event& _event) {
	MenuState::getInput(_event);
	m_IPPrompt.getInput(_event);
}

void ProgramState_MPJoinMenu::update(int _timeslice) {
	MenuState::update(_timeslice);
	m_IPPrompt.update();
	*m_IPStr = m_IPPrompt.getString();
}

void ProgramState_MPJoinMenu::draw() const {
	MenuState::draw();
	m_window.draw(m_IPPrompt);
}

void ProgramState_MPJoinMenu::onResize(sf::Vector2u _newSize) {
	MenuState::onResize(_newSize);
	m_IPPrompt.setPosition({m_window.getSize().x / 2.F,
							m_window.getSize().y / 2.5F});
}
