#ifndef PROGRAMSTATE_MENU_H_INCLUDED
#define PROGRAMSTATE_MENU_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "ProgramState.h"

class ProgramState_Menu : public ProgramState {
  public:
	ProgramState_Menu(Program& _program,
	                  bool     _visibleOverPreviousState = false);
	~ProgramState_Menu();

	virtual void getInput(sf::Event& _event);
	virtual void update(int _timeslice);
	virtual void draw();

	void addMenuItem(const std::string& _string,
	                 void (Program::*f)() = nullptr,
	                 int _keyCode         = sf::Keyboard::Unknown);
	void addGap();

  private:
	//Functions -----------------------------------
	void detectMouseClicks();
	void updateTitleText(int _timeslice);
  	//---------------------------------------------

  protected:
	//Functions -----------------------------------
	virtual void onResize(sf::Vector2u _newSize);
  	//---------------------------------------------

  protected:
	typedef std::tuple<bool, void (Program::*)(), sf::Text, int> MenuItem;

	//Data members --------------------------------
	bool isMousedOver(MenuItem _menuItem);
	bool isFunctionNull(MenuItem _menuItem);
	//We'll represent the menu items with a vector of tuples.
	//
	//The first element, a boolean, represents whether the
	//element is currently being moused over or not
	//
	//The second element, a function pointer, will be used to
	//determine what happens when the item is clicked on
	//
	//The third element, an sf::Text object, is what the user
	//sees on the menu screen
	//
	//The fourth and final element will be an integer representing
	//a key (sf::Keyboard). This is optional and can be used as a
	//shortcut to simulate a click on this MenuItem

	std::vector<MenuItem> m_menuItems;
	sf::View              m_view;

	static sf::Text m_titleText;
  	//---------------------------------------------

};

#endif // PROGRAMSTATE_MENU_H_INCLUDED
