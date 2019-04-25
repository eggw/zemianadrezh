#include "ProgramState_Play.h"

#include "Keybinds.h"
#include "LoggerNetwork.h"

#include <iostream>

ProgramState_Play::ProgramState_Play(Program& _program,
									 sf::IpAddress _ipAddress)
	: 	ProgramState(_program),
	   //set the last argument of m_client's constructor
	   //to nullptr to test a non-local (non-resource sharing)
	   //instance of Server and Client
	   m_client(*m_program.m_window,
				_ipAddress,
				m_program.getServer()),
	   m_rendererChunk(*m_program.m_window),
	   m_rendererClientPlayer(*m_program.m_window),
	   m_rendererChatbox(*m_program.m_window),
	   m_rendererOtherPlayers(*m_program.m_window),
	   m_rendererUserInterface(*m_program.m_window),
	   m_rendererDayNightCycle(*m_program.m_window),
	   m_dayNightCycle(m_client.getWorld()),
	   m_view{sf::FloatRect(0, 0,
							float(m_program.m_window->getSize().x),
							float(m_program.m_window->getSize().y))},

	   m_skyView{{float(m_program.m_window->getSize().x/2), float(m_program.m_window->getSize().y/2)},
				{float(m_program.m_window->getSize().x), float(m_program.m_window->getSize().y)}} {


	m_client.m_networkManager.connect(_ipAddress, Packet::Port_TCP_Server);

	m_rendererChatbox.addObject(m_client.getChatBox());
	m_rendererClientPlayer.addObject(m_client.getPlayer());
	m_rendererUserInterface.addObject(m_client.getUserInterface());
	m_rendererDayNightCycle.addObject(&m_dayNightCycle);

}

ProgramState_Play::~ProgramState_Play() {

}

void ProgramState_Play::getInput(sf::Event& _event) {
	ProgramState::getInput(_event);

	if(_event.type == sf::Event::KeyPressed) {
		if(_event.key.code == Key::PAUSE_KEY) {
			m_program.pushState_Pause();
			return;
		}
	}

	else if (_event.type == sf::Event::MouseButtonPressed) {
		if (_event.key.code == sf::Mouse::Left) {
			//Temporary; to be used later
			sf::Vector2i coords{ sf::Mouse::getPosition(*m_program.m_window) };
			sf::Vector2i coordsInWorld{ m_program.m_window->mapPixelToCoords(coords) };
			std::cout << "LMB pressed at " << coordsInWorld.x << ", " << coordsInWorld.y << std::endl;
		}
	}

	m_client.getInput(_event);
}

void ProgramState_Play::update(int _timeslice) {
	m_dayNightCycle.update();
	m_view.setCenter(m_client.getPlayer()->getPosition());

	m_client.receivePackets();

	renderUpdatedChunks();
	renderNewPlayers();

	m_client.update(_timeslice);
	m_client.sendPackets();

	if(!m_client.isConnected()) {
        m_program.pushState_MPConnectionLost();
	}
}

void ProgramState_Play::draw() {
	m_program.m_window->setView(m_skyView);
	m_rendererDayNightCycle.draw();

	m_program.m_window->setView(m_view);
	m_rendererClientPlayer.draw();
	m_rendererOtherPlayers.draw();
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

	m_skyView = sf::View({float(_newSize.x/2), float(_newSize.y/2)},
							{float(_newSize.x), float(_newSize.y)});
}

void ProgramState_Play::renderUpdatedChunks() {
	std::unique_ptr<std::vector<int>> ptr(new std::vector<int>);

	if(m_client.m_networkManager.chunkDataReceived(ptr.get())) {
		auto worldData = m_client.getWorld().getChunks();
		for(const auto& chunk : worldData) {
			auto result = std::find(std::begin(*ptr),
									std::end(*ptr),
									chunk.getID());

			if(result != std::end(*ptr)) {
				m_rendererChunk.update(&chunk);
			}
		}

		m_client.m_networkManager.setChunkDataProcessed(true);
	}
}

//TODO: refactor this shit
void ProgramState_Play::renderNewPlayers() {
	for(const auto& player : *m_client.getPlayers()) {
		if(player->getName() != m_client.getPlayer()->getName()) {
			m_rendererOtherPlayers.addObject(player);
		}
	}
}
//-------------------
