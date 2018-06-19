#ifndef PROGRAMSTATE_MENU_H_INCLUDED
#define PROGRAMSTATE_MENU_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "ProgramState.h"

class ProgramState_Menu : public ProgramState {
    public:
        ProgramState_Menu(Program& _program);
        ~ProgramState_Menu();

        void getInput();
        void update();
        void draw();

        void addTextItem(const std::string _string);

    protected:
    	std::vector<sf::Text> m_texts;
};

#endif // PROGRAMSTATE_MENU_H_INCLUDED
