#include "ProgramState_MPJoinMenu.h"

ProgramState_MPJoinMenu::ProgramState_MPJoinMenu(Program& _program)
    :ProgramState_Menu(_program) {

    addMenuItem("Test");

    addGap();
    addMenuItem("Back to Main Menu",
                &Program::returnToMainMenu);

}
