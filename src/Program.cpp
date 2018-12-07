#include "Program.h"

#include "ProgramStates/ProgramState_Play.h"
#include "ProgramStates/ProgramState_Pause.h"
#include "ProgramStates/ProgramState_MainMenu.h"
#include "ProgramStates/ProgramState_MPMenu.h"
#include "ProgramStates/ProgramState_MPHostMenu.h"
#include "ProgramStates/ProgramState_MPJoinMenu.h"
#include "ProgramStates/ProgramState_MPJoinFailed.h"
#include "ProgramStates/ProgramState_ConnectionLost.h"

constexpr int FIXED_TIMESLICE { 16 };

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
	int simulationTime { 0 }; //as milliseconds
	sf::Clock timesliceClock{};

	while(m_window->isOpen()) {
		m_window->clear(sf::Color(53,80,200));

        getInput();

		int realTime{ timesliceClock.getElapsedTime().asMilliseconds() };
		while (simulationTime < realTime){
			simulationTime += FIXED_TIMESLICE;

			if(localServerInitialised()) {
				m_localServer->receivePackets();
				m_localServer->update(FIXED_TIMESLICE);
			}

			update(FIXED_TIMESLICE);
		}

		draw();
	}
}

void Program::getInput() {
    sf::Event event;
    while(m_window->pollEvent(event)) {
        if(event.type == sf::Event::Closed) {
            m_window->close();
        }

        //TODO
        //At the moment, pressing "Escape" while paused will
        //create a second escape menu on top of the existing
        //one, thus preventing the closing of the pause menu
        //to work using ProgramState::Menu's keyboard shortcuts.

        //All I really want is for the previous state to fix itself
        //if the screen is resized. I'll revisit this later.

        //if(m_states.back()->isVisibleOverPreviousState()) {
			//m_states.end()[-2]->getInput(event);
        //}

        m_states.back()->getInput(event);
    }
}

void Program::update(int _timeslice) {
	if(m_states.back()->isVisibleOverPreviousState()) {
		m_states.end()[-2]->update(_timeslice);
	}
	m_states.back()->update(_timeslice);
}

void Program::draw() {
	if(m_states.back()->isVisibleOverPreviousState()) {
		m_states.end()[-2]->draw();
	}
	m_states.back()->draw();
	m_window->display();
}

void Program::pushState_Play_SP() {
	initialiseLocalServer(false);
	m_states.push_back(std::unique_ptr<ProgramState_Play>(new ProgramState_Play(*this,
					   sf::IpAddress::LocalHost)));
}

void Program::pushState_Play_MP_Host() {
	initialiseLocalServer(true);
	m_states.push_back(std::unique_ptr<ProgramState_Play>(new ProgramState_Play(*this,
					   sf::IpAddress::LocalHost)));
}

void Program::pushState_Play_MP_Join() {
	sf::IpAddress ip{sf::IpAddress(m_ipAddress)};
	std::unique_ptr<ProgramState_Play>newState (new ProgramState_Play(*this,
													m_ipAddress));
	if(newState->clientConnected()){
		m_states.push_back(std::move(newState));
	}
	else{
		pushState_MPJoinFailed();
	}
}

void Program::pushState_Pause() {
	m_states.push_back(std::unique_ptr<ProgramState_Pause>(new ProgramState_Pause(*this)));
}

void Program::pushState_MPMenu() {
	m_states.push_back(std::unique_ptr<ProgramState_MPMenu>(new ProgramState_MPMenu(*this)));
}

void Program::pushState_MPHostMenu() {
	m_states.push_back(std::unique_ptr<ProgramState_MPHostMenu>(new ProgramState_MPHostMenu(*this)));
}

void Program::pushState_MPJoinMenu() {
	m_states.push_back(std::unique_ptr<ProgramState_MPJoinMenu>(new ProgramState_MPJoinMenu(*this)));
}

void Program::pushState_MPJoinFailed() {
	m_states.push_back(std::unique_ptr<ProgramState_MPJoinFailed>(new ProgramState_MPJoinFailed(*this, m_ipAddress)));
}

void Program::pushState_MPConnectionLost() {
	m_states.push_back(std::unique_ptr<ProgramState_ConnectionLost>(new ProgramState_ConnectionLost(*this)));
}

bool Program::localServerInitialised() {
	return m_localServer != nullptr;
}

void Program::initialiseLocalServer(bool _joinable) {
	m_localServer = std::unique_ptr<Server>(new Server(_joinable));
}

void Program::terminateLocalServer() {
	m_localServer = nullptr;
}

bool Program::isAtMainMenu() {
	ProgramState_MainMenu* ptrTest = dynamic_cast<ProgramState_MainMenu*> (m_states.back().get());
	return (ptrTest != nullptr);
}

//This function simply keeps popping the state until it's the main menu state.
void Program::returnToMainMenu() {
	while(!isAtMainMenu()) {
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

void Program::setIpAddress(const std::string& _ipStr) {
	m_ipAddress = _ipStr;
}

Server* Program::getServer() const {
	if(m_localServer == nullptr) {
		return nullptr;
	}
	return m_localServer.get();
}


