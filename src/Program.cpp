#include "Program.h"

#include "ProgramState_Play.h"

Program::Program() {
    m_window = std::unique_ptr<sf::RenderWindow>
               (new sf::RenderWindow(sf::VideoMode(800,600),
                                     "Hmmmm"));
    m_window->setFramerateLimit(60);

	m_states.push(std::unique_ptr<ProgramState_Play>(new ProgramState_Play(*this)));

    gameLoop();
}

Program::~Program() {

}

void Program::init() {

}

void Program::gameLoop() {
    while(m_window->isOpen()) {
        m_window->clear(sf::Color::Black);

        m_states.top()->getInput();
        m_states.top()->update();
        m_states.top()->draw();

        m_window->display();
    }
}






