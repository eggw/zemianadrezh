#include "ProgramStates/ProgramState_MPJoinMenu.h"

#include "FontManager.h"

ProgramState_MPJoinMenu::ProgramState_MPJoinMenu(Program& _program)
            : MenuState(_program.m_window,
						FontManager::get_instance().getFont(FontManager::Type::ANDY),
						"Multiplayer - Join"),
						m_textEntryView{},
						m_IPEntry(FontManager::get_instance().getFont(FontManager::Type::ANDY),
								  200,
								  "localhost") {

	m_IPEntry.setCharSize(22);
	m_IPEntry.setMaxChars(45);
	m_IPEntry.setAlwaysVisible(true);
	m_IPEntry.setAlwaysActive(true);

	m_IPStr = std::make_unique<std::string>("");

	addGap();
	addMenuItem(
	  "Connect", std::bind(&Program::pushState_Play_MP_Join, &_program, m_IPStr.get()), sf::Keyboard::Enter);
	addGap();
	addMenuItem("Back to Main Menu", std::bind(&Program::returnToMainMenu, &_program));

	onResize(_program.m_window.getSize());
}

void ProgramState_MPJoinMenu::getInput(sf::Event& _event) {
	MenuState::getInput(_event);
	m_IPEntry.getInput(_event);
}

void ProgramState_MPJoinMenu::update(int _timeslice) {
	MenuState::update(_timeslice);
	m_IPEntry.update();
	*m_IPStr = m_IPEntry.getCurrentString();
}

void ProgramState_MPJoinMenu::draw() {
	MenuState::draw();
	m_window.setView(m_textEntryView);
	m_window.draw(m_IPEntry);
}

void ProgramState_MPJoinMenu::onResize(sf::Vector2u _newSize) {
	//TODO: figure out a more elegant way to position m_IPEntry
	MenuState::onResize(_newSize);
	m_textEntryView.reset({0,0, float(_newSize.x), float(_newSize.y)});
	m_IPEntry.setPosition({float(m_window.getSize().x) / 2, float(m_window.getSize().y / 2.5)});
}
