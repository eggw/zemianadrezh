#include "ProgramState_Play.h"
#include <iostream>

ProgramState_Play::ProgramState_Play(Program& _program)
	: 	ProgramState(_program),
	   m_localServer(),
	   m_rendererChunk(*m_program.m_window) {

	//m_serverConnection.setBlocking(false);

	m_localServer.listen();
	sf::Socket::Status status = m_serverConnection.connect(sf::IpAddress::LocalHost, 7777);\
	m_localServer.accept();
	if (status != sf::Socket::Done) {
		std::cout << "Error!" << std::endl;
	}
	else {
		std::cout << "CLIENT: Success!" << std::endl;
		sf::Packet worldPacket;
		m_serverConnection.receive(worldPacket);
	}

	auto worldChunks = m_localServer.getWorld().getChunks();

	for(auto& chunk : worldChunks) {
		m_rendererChunk.update(&chunk);
	}
}

ProgramState_Play::~ProgramState_Play() {

}

void ProgramState_Play::getInput() {
	sf::Event event;

	while(m_program.m_window->pollEvent(event)) {

		if(event.type == sf::Event::Closed) {
			m_program.m_window->close();
		}

		if(event.type == sf::Event::KeyPressed) {
			if(event.key.code == sf::Keyboard::Escape) {
				m_program.m_window->close();
			}
		}

	}

}

void ProgramState_Play::update() {

}

void ProgramState_Play::draw() {
	m_rendererChunk.draw();
}
