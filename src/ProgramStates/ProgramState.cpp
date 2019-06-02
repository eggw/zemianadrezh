#include "ProgramState.h"

constexpr float MIN_WIDTH{800};
constexpr float MIN_HEIGHT{600};

ProgramState::ProgramState(Program& _p, bool _visibleOverPreviousState)
            : m_program{_p},
              m_visibleOverPreviousState{_visibleOverPreviousState} {
}

ProgramState::~ProgramState() {
}

void ProgramState::getInput(sf::Event& _event) {
	switch (_event.type) {
	case sf::Event::Resized: {
		sf::Vector2u newSize{_event.size.width, _event.size.height};
		if (newSize.x < MIN_WIDTH) {
			newSize.x = MIN_WIDTH;
		}
		if (newSize.y < MIN_HEIGHT) {
			newSize.y = MIN_HEIGHT;
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
	onResize(m_program.m_window.getSize());
}

void ProgramState::onResize(sf::Vector2u _newSize) {
	m_program.m_window.setSize(_newSize);
}
