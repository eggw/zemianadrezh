#include "Program.h"

#include "ProgramStates/ProgramState_Play.h"
#include "ProgramStates/ProgramState_Pause.h"
#include "ProgramStates/ProgramState_MainMenu.h"
#include "ProgramStates/ProgramState_MPMenu.h"
#include "ProgramStates/ProgramState_MPHostMenu.h"
#include "ProgramStates/ProgramState_MPJoinMenu.h"
#include "ProgramStates/ProgramState_MPJoinFailed.h"
#include "ProgramStates/ProgramState_ConnectionLost.h"

constexpr const int FIXED_TIMESLICE{16};

Program::Program(sf::RenderWindow& _window) : Core(_window, FIXED_TIMESLICE) {
	pushState(
	  std::unique_ptr<ProgramState_MainMenu>(new ProgramState_MainMenu(*this)));
	run();
}

Program::~Program() {
}

void Program::run() {
	int       simulationTime{0}; //as milliseconds
	sf::Clock timesliceClock{};

	while (m_window.isOpen() && getTopState() != nullptr) {
		m_window.clear(sf::Color(0, 0, 0));
		getInput();
		int realTime{timesliceClock.getElapsedTime().asMilliseconds()};
		while (simulationTime < realTime) {
			simulationTime += FIXED_TIMESLICE;
			update(FIXED_TIMESLICE);

			//The way packet sending between client and server works is
			//as follows:
			//
			//(*)The client sends its player data to the server
			//    [NetworkManagerClient] -> [NetworkManagerServer]
			//
			//(*) The server then sets its copy of the player according
			//to the data received.
			//
			//When sharing GameInstance::m_world & ::m_registry with
			//a local server, the exchange takes place virtually instantly.
			//
			//For this reason, it is important that the client updates
			//and gets input just before the local server does.

			if (localServerInitialised()) {
				if (!isPaused() || m_localServer->connectionsAllowed()) {
					m_localServer->acceptConnections();
					m_localServer->receivePackets();
					m_localServer->update(FIXED_TIMESLICE);
				}
			}
		}

		draw();
	}
}

void Program::pushState_Play_SP() {
	initialiseLocalServer(false);
	pushState(std::unique_ptr<ProgramState_Play>(
	  new ProgramState_Play(*this, sf::IpAddress::LocalHost, m_localServer.get())));
}

void Program::pushState_Play_MP_Host() {
	initialiseLocalServer(true);
	pushState(std::unique_ptr<ProgramState_Play>(
	  new ProgramState_Play(*this, sf::IpAddress::LocalHost, m_localServer.get())));
}

void Program::pushState_Play_MP_Join(std::string* _ip) {
	std::unique_ptr<ProgramState_Play> newState(
	  new ProgramState_Play(*this, *_ip));

	if (newState->clientConnected()) {
		pushState(std::move(newState));
	}
	else {
		pushState_MPJoinFailed(_ip);
	}
}

void Program::pushState_Pause() {
	pushState(
	  std::unique_ptr<ProgramState_Pause>(new ProgramState_Pause(*this)));
}

void Program::pushState_MPMenu() {
	pushState(
	  std::unique_ptr<ProgramState_MPMenu>(new ProgramState_MPMenu(*this)));
}

void Program::pushState_MPHostMenu() {
	pushState(std::unique_ptr<ProgramState_MPHostMenu>(
	  new ProgramState_MPHostMenu(*this)));
}

void Program::pushState_MPJoinMenu() {
	pushState(std::unique_ptr<ProgramState_MPJoinMenu>(
	  new ProgramState_MPJoinMenu(*this)));
}

void Program::pushState_MPJoinFailed(std::string* _ip) {
	pushState(std::unique_ptr<ProgramState_MPJoinFailed>(
	  new ProgramState_MPJoinFailed(*this, *_ip)));
}

void Program::pushState_MPConnectionLost() {
	pushState(std::unique_ptr<ProgramState_ConnectionLost>(
	  new ProgramState_ConnectionLost(*this)));
}

bool Program::localServerInitialised() const {
	return m_localServer != nullptr;
}

void Program::initialiseLocalServer(bool _joinable) {
	m_localServer = std::make_unique<Server>(_joinable);
}

bool Program::isAtMainMenu() const {
	const ProgramState_MainMenu* ptrTest =
	  dynamic_cast<const ProgramState_MainMenu*>(getTopState());
	return (ptrTest != nullptr);
}

bool Program::isPaused() const {
	const ProgramState_Pause* ptrTest =
	  dynamic_cast<const ProgramState_Pause*>(getTopState());
	return (ptrTest != nullptr);
}

//This function simply keeps popping the state until it's the main menu state.
//It additionally resets the local server.
void Program::returnToMainMenu() {
	while (!isAtMainMenu()) {
		popState();
	}
	m_localServer.reset();
}
