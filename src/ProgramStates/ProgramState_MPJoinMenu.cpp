#include "ProgramState_MPJoinMenu.h"

#include "FontManager.h"

//Here are the variables we'll use to initialise the IP entry box
//in the correct position. It will take 20% of the screen width and
//be located just above the first item in m_menuItems (which should be
//"Connect" in this case)
//////////////////////////////////////////////////////////////////////////////////////////////
constexpr unsigned int IPENTRY_CHARACTER_SIZE{20};
constexpr float        IPENTRY_WIDTH_FACTOR{0.2f};
constexpr float        IPENTRY_X_POS{0.5f - (IPENTRY_WIDTH_FACTOR / 2)};
//////////////////////////////////////////////////////////////////////////////////////////////

ProgramState_MPJoinMenu::ProgramState_MPJoinMenu(Program& _program)
            : ProgramState_Menu(_program), m_IPEntry(18, 45) {
	addMenuItem(
	  "Connect", &Program::pushState_Play_MP_Join, sf::Keyboard::Enter);

	addGap();
	addMenuItem("Back to Main Menu", &Program::returnToMainMenu);

	onResize(_program.m_window->getSize());

	m_IPEntry.setAlwaysVisible(true);
	m_IPEntry.setAlwaysActive(true);
	m_IPEntry.setText("localhost");
}

void ProgramState_MPJoinMenu::getInput(sf::Event& _event) {
	ProgramState_Menu::getInput(_event);
	m_IPEntry.getInput(_event);
}

void ProgramState_MPJoinMenu::update(int _timeslice) {
	ProgramState_Menu::update(_timeslice);
	m_IPEntry.update();
	m_program.setIpAddress(m_IPEntry.getCurrentString());
}

void ProgramState_MPJoinMenu::draw() {
	m_program.m_window->draw(m_IPEntry);
	ProgramState_Menu::draw();
}

void ProgramState_MPJoinMenu::onResize(sf::Vector2u _newSize) {
	ProgramState_Menu::onResize(_newSize);

	float FIRST_ITEM_POS_Y{
	  std::get<2>(m_menuItems.front()).getPosition().y -
	  float(2 * (std::get<2>(m_menuItems.front()).getGlobalBounds().height))};
	float SCREEN_SIZE_Y{float(m_program.m_window->getSize().y)};

	float IPENTRY_Y_POS{FIRST_ITEM_POS_Y / SCREEN_SIZE_Y};

	sf::FloatRect ipEntryViewport{
	  IPENTRY_X_POS,
	  IPENTRY_Y_POS,
	  IPENTRY_WIDTH_FACTOR,
	  0 //this can be removed now as it's unused
	};

	m_IPEntry.initialise(m_program.m_window->getSize(), ipEntryViewport);
}
