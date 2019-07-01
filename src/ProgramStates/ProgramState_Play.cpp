#include "ProgramStates/ProgramState_Play.h"

#include "Keybinds.h"
#include "LoggerNetwork.h"

#include "TextureManager.h"

#include <iostream>

ProgramState_Play::ProgramState_Play(Program&      _program,
                                     sf::IpAddress _ipAddress,
                                     Server* _localServer)
            : State(_program.m_window),
              //set the last argument of m_client's constructor
              //to nullptr to test a non-local (non-resource sharing)
              //instance of Server and Client
              m_program{_program},
              m_client(_ipAddress, _localServer) {

	setDrawnInBackground(true);
	if(_localServer != nullptr) {
		if(_localServer->connectionsAllowed()) {
			setUpdatedInBackground(true);
		}
	}
}

ProgramState_Play::~ProgramState_Play() {
}

void ProgramState_Play::getInput(sf::Event& _event) {
	State::getInput(_event);

	if (_event.type == sf::Event::KeyPressed) {
		if (_event.key.code == Key::PAUSE_KEY) {
			m_program.pushState_Pause();
			return;
		}
	}

	else if (_event.type == sf::Event::MouseButtonPressed) {
		if (_event.key.code == sf::Mouse::Left) {
			//Temporary; to be used later
			sf::Vector2i coords{sf::Mouse::getPosition(m_program.m_window)};
			sf::Vector2i coordsInWorld{
			  m_program.m_window.mapPixelToCoords(coords)};
			std::cout << "LMB pressed at " << coordsInWorld.x << ", "
			          << coordsInWorld.y << std::endl;
		}
	}

	m_client.getInput(_event);
}

void ProgramState_Play::update(int _timeslice) {
	m_client.receivePackets();
	m_client.update(_timeslice);
	m_client.sendPlayerPacket();

	if (!m_client.isConnected()) {
		m_program.pushState_MPConnectionLost();
	}
}

void ProgramState_Play::draw() {
	m_program.m_window.draw(m_client);
}

bool ProgramState_Play::clientConnected() const {
	return m_client.isConnected();
}

void ProgramState_Play::onResize(sf::Vector2u _newSize) {
	State::onResize(_newSize);
}
