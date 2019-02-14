#include "ProgramState.h"

ProgramState::ProgramState(Program& _p, bool _visibleOverPreviousState)
	: m_program{_p},
	  m_visibleOverPreviousState{_visibleOverPreviousState} {
}

ProgramState::~ProgramState() {

}

void ProgramState::getInput(sf::Event& _event) {
	switch(_event.type) {
	case sf::Event::Resized: {
			sf::Vector2u newSize {_event.size.width, _event.size.height};
			if (newSize.x < 800) {
				newSize.x = 800;
			}
			if (newSize.y < 600) {
				newSize.y = 600;
			}
			onResize(newSize);
			break;
		}
	}
}

bool ProgramState::isVisibleOverPreviousState() const {
	return m_visibleOverPreviousState;
}

void ProgramState::onStateSwitch() {
	onResize(m_program.m_window->getSize());
}

void ProgramState::onResize(sf::Vector2u _newSize) {
	m_program.m_window->setSize(_newSize);
}
