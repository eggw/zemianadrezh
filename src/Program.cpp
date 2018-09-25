#include "Program.h"

#include "ProgramState_Play.h"
#include "ProgramState_Pause.h"
#include "ProgramState_MainMenu.h"
#include "ProgramState_MPMenu.h"

Program::Program() {
    m_window = std::unique_ptr<sf::RenderWindow>
               (new sf::RenderWindow(sf::VideoMode(800,600),
                                     "zemianadrezh"));
    m_window->setFramerateLimit(60);

    m_states.push_back(std::unique_ptr<ProgramState_MainMenu>(new ProgramState_MainMenu(*this)));

    gameLoop();
}

Program::~Program() {

}

void Program::init() {

}

void Program::gameLoop() {
    while(m_window->isOpen()) {
        m_window->clear(sf::Color(53,80,200));

        //Monitor window closing
        /////////////////////////////////////////////////////
        sf::Event event;
        while(m_window->pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                m_window->close();
            }

            m_states.back()->getInput(event);
        }
        /////////////////////////////////////////////////////

        m_states.back()->update();

        if(m_states.back()->isVisibleOverPreviousState()){
            m_states.end()[-2]->draw();
        }
        m_states.back()->draw();

        m_window->display();
    }
}

void Program::pushState_Play() {
    m_states.push_back(std::unique_ptr<ProgramState_Play>(new ProgramState_Play(*this)));
}

void Program::pushState_Pause(){
    m_states.push_back(std::unique_ptr<ProgramState_Pause>(new ProgramState_Pause(*this)));
}

void Program::pushState_MPMenu(){
    m_states.push_back(std::unique_ptr<ProgramState_MPMenu>(new ProgramState_MPMenu(*this)));
}

bool Program::isAtMainMenu(){
    ProgramState_MainMenu* ptrTest = dynamic_cast<ProgramState_MainMenu*> (m_states.back().get());
    return (ptrTest != nullptr);
}

//This function simply keeps popping the state until it's the main menu state.
void Program::returnToMainMenu(){
    while(!isAtMainMenu()){
        popState();
      }
}

void Program::popState() {
    m_states.pop_back();

    if(!m_states.empty()) {
        m_states.back()->onStateSwitch();
    }
}

void Program::closeWindow() {
    m_window->close();
}




