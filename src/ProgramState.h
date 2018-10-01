#ifndef PROGRAMSTATE_H_INCLUDED
#define PROGRAMSTATE_H_INCLUDED

#include "SFML/Graphics.hpp"

#include "Program.h"

class ProgramState {

    public:
        ProgramState(Program& _p, bool _visibleOverPreviousState = false);
        virtual ~ProgramState() = 0;

        virtual void getInput(sf::Event& _event);
        virtual void update() = 0;
        virtual void draw() = 0;

        bool isVisibleOverPreviousState() const;

        //onStateSwitch() is called by the new current state
        //when a state is popped from the states stack.
        //By default, all it does is call the onResize() function,
        //ensuring the views aren't all messed up
        virtual void onStateSwitch();

    protected:
        virtual void onResize(sf::Vector2u _newSize);

        Program& m_program;
        bool m_visibleOverPreviousState;
};

#endif // PROGRAMSTATE_H_INCLUDED
