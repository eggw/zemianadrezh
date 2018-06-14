#ifndef PROGRAMSTATE_H_INCLUDED
#define PROGRAMSTATE_H_INCLUDED

#include "SFML/Graphics.hpp"

#include "Program.h"

class ProgramState {

    public:
        ProgramState(Program& _p);
        virtual ~ProgramState() = 0;

        virtual void getInput() = 0;
        virtual void update() = 0;
        virtual void draw() = 0;

    protected:
        Program& m_program;
};

#endif // PROGRAMSTATE_H_INCLUDED
