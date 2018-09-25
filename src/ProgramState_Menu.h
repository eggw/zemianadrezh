#ifndef PROGRAMSTATE_MENU_H_INCLUDED
#define PROGRAMSTATE_MENU_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "ProgramState.h"

class ProgramState_Menu : public ProgramState {
    public:
        ProgramState_Menu(Program& _program, bool _visibleOverPreviousState = false);
        ~ProgramState_Menu();

        void getInput(sf::Event& _event);
        void update();
        void draw();

        void addMenuItem(const std::string _string, void(Program::*f)() = nullptr);
        void addGap();

    private:
        void onResize(sf::Vector2u _newSize);
    private:
        typedef std::tuple<bool, void(Program::*)(), sf::Text> MenuItem;
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
        //The last element, an sf::Text object, is what the user
        //sees on the menu screen
        std::vector<MenuItem> m_menuItems;
        sf::View m_view;
};

#endif // PROGRAMSTATE_MENU_H_INCLUDED
