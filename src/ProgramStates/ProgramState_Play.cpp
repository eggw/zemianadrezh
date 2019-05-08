#include "ProgramState_Play.h"

#include "Keybinds.h"
#include "LoggerNetwork.h"

#include "TextureManager.h"

#include <iostream>

ProgramState_Play::ProgramState_Play(Program&      _program,
                                     sf::IpAddress _ipAddress)
            : ProgramState(_program),
              //set the last argument of m_client's constructor
              //to nullptr to test a non-local (non-resource sharing)
              //instance of Server and Client
              m_client(*m_program.m_window,
                       _ipAddress,
                       m_program.getServer()),
              m_rendererChunk(*m_program.m_window, m_client.m_world),
              m_rendererChatbox(*m_program.m_window),
              m_rendererUserInterface(*m_program.m_window),
              m_rendererDayNightCycle(*m_program.m_window),
              m_systemAnimation{},
              m_systemDrawing{*m_program.m_window},
              m_systemPhysics{},
              m_systemPlayerMovement{m_client.getPlayerId()},
              m_dayNightCycle(m_client.getWorld()),
              m_view{sf::FloatRect(0,
                                   0,
                                   float(m_program.m_window->getSize().x),
                                   float(m_program.m_window->getSize().y))},

              m_skyView{{float(m_program.m_window->getSize().x / 2),
                         float(m_program.m_window->getSize().y / 2)},
                        {float(m_program.m_window->getSize().x),
                         float(m_program.m_window->getSize().y)}} {
	m_client.m_networkManager.connect(_ipAddress, Packet::Port_TCP_Server);

	m_rendererChatbox.addObject(m_client.getChatBox());
	m_rendererUserInterface.addObject(m_client.getUserInterface());
	m_rendererDayNightCycle.addObject(&m_dayNightCycle);
}

ProgramState_Play::~ProgramState_Play() {
}

void ProgramState_Play::getInput(sf::Event& _event) {
	ProgramState::getInput(_event);

	if (_event.type == sf::Event::KeyPressed) {
		if (_event.key.code == Key::PAUSE_KEY) {
			m_program.pushState_Pause();
			return;
		}
	}

	else if (_event.type == sf::Event::MouseButtonPressed) {
		if (_event.key.code == sf::Mouse::Left) {
			//Temporary; to be used later
			sf::Vector2i coords{sf::Mouse::getPosition(*m_program.m_window)};
			sf::Vector2i coordsInWorld{
			  m_program.m_window->mapPixelToCoords(coords)};
			std::cout << "LMB pressed at " << coordsInWorld.x << ", "
			          << coordsInWorld.y << std::endl;
		}
	}

	m_client.getInput(_event);
	m_systemPlayerMovement.getInput(m_client.m_registry);
}

void ProgramState_Play::update(int _timeslice) {
	m_systemAnimation.update(_timeslice, m_client.m_registry);
	m_systemPhysics.update(_timeslice, m_client.m_registry);
	m_systemPlayerMovement.update(_timeslice, m_client.m_registry);

	m_dayNightCycle.update();
	m_view.setCenter(m_client.getPlayerPosition());

	m_client.receivePackets();

	renderUpdatedChunks();

	m_client.update(_timeslice);
	m_client.sendPackets();

	if (!m_client.isConnected()) {
		m_program.pushState_MPConnectionLost();
	}
}

void ProgramState_Play::draw() {
	m_program.m_window->setView(m_skyView);
	m_rendererDayNightCycle.draw();

	m_program.m_window->setView(m_view);
	m_systemDrawing.draw(m_client.m_registry);
	m_rendererChunk.draw();
	m_rendererChatbox.draw();
	m_rendererUserInterface.draw();
}

bool ProgramState_Play::clientConnected() const {
	return m_client.isConnected();
}

void ProgramState_Play::onResize(sf::Vector2u _newSize) {
	ProgramState::onResize(_newSize);
	sf::Vector2f newSizeF{float(_newSize.x), float(_newSize.y)};
	m_view.setSize(newSizeF);

	m_skyView = sf::View({float(_newSize.x / 2), float(_newSize.y / 2)},
	                     {float(_newSize.x), float(_newSize.y)});
}

void ProgramState_Play::renderUpdatedChunks() {
	auto updatedIDs {std::make_unique<std::vector<int>>()};

	if (m_client.m_networkManager.chunkDataReceived(updatedIDs.get())) {
		for(auto id : *updatedIDs) {
			m_rendererChunk.update(id);
		}
		m_client.m_networkManager.setChunkDataProcessed(true);
	}
}
