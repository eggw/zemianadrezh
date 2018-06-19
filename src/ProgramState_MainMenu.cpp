#include "ProgramState_MainMenu.h"

#include "ProgramState_Play.h"

ProgramState_MainMenu::ProgramState_MainMenu(Program& _program)
    :ProgramState_Menu(_program) {

    addTextItem("Singleplayer",
                &m_program.pushState_Play);

    addTextItem("Multiplayer");

    addTextItem("Options");

    addTextItem("Exit",
                &m_program.closeWindow);
}
