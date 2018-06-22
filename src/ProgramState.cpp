#include "ProgramState.h"

ProgramState::ProgramState(Program& _p)
    : m_program{_p} {
}

ProgramState::~ProgramState() {

}

void ProgramState::getInput(sf::Event& _event) {
    switch(_event.type) {
        case sf::Event::Resized: {
            sf::Vector2u newSize{_event.size.width, _event.size.height};
            onResize(newSize);
            break;
        }
    }
}

void ProgramState::onStateSwitch() {
    onResize(m_program.m_window->getSize());
}

void ProgramState::onResize(sf::Vector2u _newSize) {
    m_program.m_window->setSize(_newSize);
}
