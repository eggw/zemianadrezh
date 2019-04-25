#include "ProgramState_Play.h"

#include "Keybinds.h"
#include "LoggerNetwork.h"

#include <iostream>

constexpr int DAY_BEGIN_HOUR {5};
constexpr int DAY_END_HOUR {19};

constexpr int NIGHT_BEGIN_HOUR {19};
constexpr int NIGHT_END_HOUR {5};

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
	   m_rendererSunMoon(*m_program.m_window),
	   m_view{sf::FloatRect(0, 0,
							float(m_program.m_window->getSize().x),
							float(m_program.m_window->getSize().y))},

	   m_skyView{{float(m_program.m_window->getSize().x/2), float(m_program.m_window->getSize().y/2)},
				{float(m_program.m_window->getSize().x), float(m_program.m_window->getSize().y)}},

	   m_sunMoonSprite{}	{


	m_client.m_networkManager.connect(_ipAddress, Packet::Port_TCP_Server);

	m_rendererChatbox.addObject(m_client.getChatBox());
	m_rendererClientPlayer.addObject(m_client.getPlayer());
	m_rendererUserInterface.addObject(m_client.getUserInterface());

	updateSunMoon();
	m_rendererSunMoon.addObject(&m_sunMoonSprite);

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
	updateSunMoon();
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
	m_rendererSunMoon.draw();

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

void ProgramState_Play::updateSunMoon() {
	static const double pi {std::atan(1)*4};
	World::WorldTime worldTime = m_client.getWorld().getTime();

	int HOUR_START 	{DAY_BEGIN_HOUR};
	int HOUR_END 	{DAY_END_HOUR};

	if(worldTime.hours >= DAY_BEGIN_HOUR && worldTime.hours < DAY_END_HOUR) {
		m_sunMoonSprite.setTexture(TextureManager::get_instance().getTexture(TextureManager::Type::SUN));
	}
	else{
		HOUR_START 	= NIGHT_BEGIN_HOUR;
		HOUR_END 	= NIGHT_END_HOUR;
		m_sunMoonSprite.setTexture(TextureManager::get_instance().getTexture(TextureManager::Type::MOON));
	}

	//We're going to set the origin to be at the rightmost point horizontally and the
	//centermost point vertically. Later, when determining xPos, we add the width of the
	//sprite to our calculation. This is to ensure a seamless transition (at the exact
	//moment the sun is out of our view, the moon starts to come in, and vice versa)
	m_sunMoonSprite.setOrigin(	m_sunMoonSprite.getGlobalBounds().width,
								m_sunMoonSprite.getGlobalBounds().height / 2);

	//These two if statements make sure that everything runs smoothly in cases such as the following:
	//HOUR_START = 19
	//HOUR_END = 5
	//worldTime.hours = 0
	//In such a case, the algorithm will get confused and not work as it should because of the way
	//24h wrapping works. Adding 24 will fix the logic so that the correct number of hours are given
	if(HOUR_START > HOUR_END) {
        HOUR_END += 24;
	}
	if(worldTime.hours < HOUR_START) {
		worldTime.hours += 24;
	}

	//Get the "percentage" of how far along the X axis the sun should be (for example, at 12, it should be
	//exactly 50% of the way.
	//First, calculate the difference between the hours.
	//[For example, from 6 to 18, this would be 12]
	int difference {abs(HOUR_START - HOUR_END)};
	//Then, calculate the offset from the begin hour.
	//[For example, if we take 12, 12 - 6 = 6]
	int offset {abs(HOUR_START - worldTime.hours)};
	//We'll now be able to take into consideration the minutes. We simply divide
	//the world time in minutes by 60 to get the percentage value between our
	//current hour and the next hour. For example, 30 would yield 0.5.
	float minutesPercentage {worldTime.minutes / 60.f };

	//We can divide our minutesPercentage by the hour difference to get the value we should
	//increment our final result by.
	float minuteProgress {minutesPercentage / difference};

	//Finally, divide the offset by the difference to get the percentage value for the hours, then
	//increment it by the minutes percentage.
	double progressInPercent {double(offset) / double(difference)};
	progressInPercent += minuteProgress;

	//For the xPos, we're going to want to invert progressInPercent so that the sun/moon goes
	//from the east to the west and not vice versa
	//Also, since the sprite's X origin is at its rightmost point, we'll add the sprite's width to
	//the view size in our xPos calculation

	//We're also going to invert the sin result in yPos so the movement will be down->up->down, rather
	//than up->down->up
	double xPos {(1 - progressInPercent) * (m_skyView.getSize().x + m_sunMoonSprite.getGlobalBounds().width)};
	double yPos {(1 - sin(progressInPercent * pi)) * m_skyView.getSize().y / 3};
	//This adjustment was made with some experimenting to get it to look right
	yPos += m_skyView.getSize().y / 10;

	m_sunMoonSprite.setPosition(xPos, yPos);

}
