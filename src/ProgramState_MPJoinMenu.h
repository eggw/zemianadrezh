#ifndef PROGRAMSTATE_MPJOINMENU_H_INCLUDED
#define PROGRAMSTATE_MPJOINMENU_H_INCLUDED

#include "ProgramState_Menu.h"

#include "TextEntryBox.h"

class ProgramState_MPJoinMenu : public ProgramState_Menu {
	public:
		ProgramState_MPJoinMenu (Program& _program);
		void getInput(sf::Event& _event);
		void update();
		void draw();
	private:
		TextEntryBox m_IPEntry;
};

#endif // PROGRAMSTATE_MPJOINMENU_H_INCLUDED
