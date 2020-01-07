#ifndef PROGRAMSTATE_MPJOINMENU_H_INCLUDED
#define PROGRAMSTATE_MPJOINMENU_H_INCLUDED

#include <SPSS/System/MenuState.h>
#include <SPSS/Graphics/DialogPrompt.h>

#include "Program.h"

class ProgramState_MPJoinMenu : public spss::MenuState {
  public:
	ProgramState_MPJoinMenu(Program& _program);
	void getInput(sf::Event& _event);
	void update(int _timeslice);
	void draw() const;

  protected:
	//Functions -----------------------------------
	void onResize(sf::Vector2u _newSize);
	//---------------------------------------------

  private:
	//Data members --------------------------------

	/////////////////////////////////////////////////////////////////////
	/// When we bind "Connect" to Program::pushState_Play_MP_Join,
	/// we'll pass a ptr to the IP string, because its value will
	/// be different by the time the user actually enters the IP
	/// and clicks it
	/////////////////////////////////////////////////////////////////////
	std::unique_ptr<std::string> m_IPStr;
	spss::DialogPrompt           m_IPPrompt;
	//---------------------------------------------
};

#endif // PROGRAMSTATE_MPJOINMENU_H_INCLUDED
