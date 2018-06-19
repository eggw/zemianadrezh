#ifndef PROGRAMSTATE_MAINMENU_H_INCLUDED
#define PROGRAMSTATE_MAINMENU_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "ProgramState_Menu.h"

class ProgramState_MainMenu : public ProgramState_Menu {
    public:
        ProgramState_MainMenu(Program& _program);
        ~ProgramState_MainMenu();

        void getInput();
        void update();
        void draw();
};

#endif // PROGRAMSTATE_MAINMENU_H_INCLUDED
