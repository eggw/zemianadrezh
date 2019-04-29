#include "DayNightCycle.h"

#include "TextureManager.h"
#include "FontManager.h"

#include <iostream>

constexpr int DAY_BEGIN_HOUR{5};
constexpr int DAY_END_HOUR{19};

constexpr int NIGHT_BEGIN_HOUR{19};
constexpr int NIGHT_END_HOUR{5};

DayNightCycle::DayNightCycle(const World& _world)
            : m_world{_world},
              m_sunMoonSprite{},
              m_skyBackground{sf::PrimitiveType::Triangles, 42},
              m_timeText{},
              m_target{nullptr},
              m_shader{} {
	m_timeText.setFont(
	  FontManager::get_instance().getFont(FontManager::Type::ANDY));
	m_timeText.setCharacterSize(30);
	m_timeText.setOutlineThickness(1);
	m_timeText.setOrigin(m_timeText.getGlobalBounds().width / 2,
	                     m_timeText.getGlobalBounds().height / 2);

	//making the sky blue (temporary until shader is implemented properly)
	/*
	for(int i{0}; i < 4; ++i) {
	    m_skyBackground[i].color = sf::Color(90,112,255);
	}
	*/

	if (!m_shader.loadFromFile("assets/shaders/cycle_vertex.glsl",
	                           "assets/shaders/cycle_fragment.glsl")) {
		std::cout << "ERROR!" << std::endl;
	}

	m_shader.setUniform("sky",
	                    TextureManager::get_instance().getTexture(
	                      TextureManager::Type::CYCLE_SKY_GRADIENT));
	m_shader.setUniform("glow",
	                    TextureManager::get_instance().getTexture(
	                      TextureManager::Type::CYCLE_GLOW_GRADIENT));

	update();
}

void DayNightCycle::update() {
	//A draw call needs to be attempted at least once to set our target
	//before updating can begin
	if (m_target == nullptr) {
		return;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma)) {
		m_world.getTime().pause();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period)) {
		m_world.getTime().unpause();
	}

	updateSkyVertices();

	static const double pi{std::atan(1) * 4};
	auto                worldTime{m_world.getTime()};
	auto                hhmm{worldTime.get()};

	m_timeText.setString(worldTime.getString());
	m_timeText.setPosition(
	  m_target->getSize().x - 1.2 * m_timeText.getGlobalBounds().width, 0);

	int HOUR_START{DAY_BEGIN_HOUR};
	int HOUR_END{DAY_END_HOUR};

	if (hhmm.hours >= DAY_BEGIN_HOUR && hhmm.hours < DAY_END_HOUR) {
		m_sunMoonSprite.setTexture(
		  TextureManager::get_instance().getTexture(TextureManager::Type::SUN));
	}
	else {
		HOUR_START = NIGHT_BEGIN_HOUR;
		HOUR_END   = NIGHT_END_HOUR;
		m_sunMoonSprite.setTexture(TextureManager::get_instance().getTexture(
		  TextureManager::Type::MOON));
	}

	//We're going to set the origin to be at the rightmost point horizontally and the
	//centermost point vertically. Later, when determining xPos, we add the width of the
	//sprite to our calculation. This is to ensure a seamless transition (at the exact
	//moment the sun is out of our view, the moon starts to come in, and vice versa)
	m_sunMoonSprite.setOrigin(m_sunMoonSprite.getGlobalBounds().width,
	                          m_sunMoonSprite.getGlobalBounds().height / 2);

	//These two if statements make sure that everything runs smoothly in cases such as the following:
	//HOUR_START = 19
	//HOUR_END = 5
	//worldTime.hours = 0
	//In such a case, the algorithm will get confused and not work as it should because of the way
	//24h wrapping works. Adding 24 will fix the logic so that the correct number of hours is given
	if (HOUR_START > HOUR_END) {
		HOUR_END += 24;
	}
	if (hhmm.hours < HOUR_START) {
		hhmm.hours += 24;
	}

	//Get the "percentage" of how far along the X axis the sun should be (for example, at 12, it should be
	//exactly 50% of the way.
	//First, calculate the difference between the hours.
	//[For example, from 6 to 18, this would be 12]
	int difference{abs(HOUR_START - HOUR_END)};
	//Then, calculate the offset from the begin hour.
	//[For example, if we take 12, 12 - 6 = 6]
	int offset{abs(HOUR_START - hhmm.hours)};
	//We'll now be able to take into consideration the minutes. We simply divide
	//the world time in minutes by 60 to get the percentage value between our
	//current hour and the next hour. For example, 30 would yield 0.5.
	float minutesPercentage{hhmm.minutes / 60.f};

	//We can divide our minutesPercentage by the hour difference to get the value we should
	//increment our final result by.
	float minuteProgress{minutesPercentage / difference};

	//Finally, divide the offset by the difference to get the percentage value for the hours, then
	//increment it by the minutes percentage.
	double progressInPercent{double(offset) / double(difference)};
	progressInPercent += minuteProgress;

	//For the xPos, we're going to want to invert progressInPercent so that the sun/moon goes
	//from the east to the west and not vice versa
	//Also, since the sprite's X origin is at its rightmost point, we'll add the sprite's width to
	//the view size in our xPos calculation

	//We're also going to invert the sin result in yPos so the movement will be down->up->down, rather
	//than up->down->up
	double xPos{
	  (1 - progressInPercent) *
	  (m_target->getSize().x + m_sunMoonSprite.getGlobalBounds().width)};
	double yPos{(1 - sin(progressInPercent * pi)) * m_target->getSize().y / 3};
	//This adjustment was made with some experimenting to get it to look right
	yPos += m_target->getSize().y / 10;

	m_sunMoonSprite.setPosition(xPos, yPos);

	//In SFML,	 the origin (0,0) is at the top left.
	//In OpenGL, the origin (0,0) is in the middle.
	//We'll do a couple of things to account for this
	sf::Vector2f openGLCoordinates{m_sunMoonSprite.getPosition()};
	//First, since the sprite's x origin is at its rightmost point, we'll
	//subtract half its width from the x coordinate.
	openGLCoordinates.x -= m_sunMoonSprite.getGlobalBounds().width / 2;
	//Taking into account openGL's texture2D origin is at the top left,
	//we'll get the appropriate coordinate position of the sun as a ratio
	//We'll divide y further in order to simulate the horizon being at the
	//center of the screen, rather than the bottom
	openGLCoordinates.x /= m_target->getSize().x;
	openGLCoordinates.y /= m_target->getSize().y / 2;

	//TODO: further refine this
	//We're going to let the shader determine the appropriate gradient from two
	//files, sky.png and glow.png, that represent the sun's position in the sky
	//over time
	m_shader.setUniform(
	  "sunPosition",
	  sf::Vector3f{openGLCoordinates.x, openGLCoordinates.y, 0.f});
	m_shader.setUniform("sunProgress", float(progressInPercent));
	m_shader.setUniform("lightIntensity", float(sin(progressInPercent * pi)));
}

void DayNightCycle::draw(sf::RenderTarget& target,
                         sf::RenderStates  states) const {
	m_target = &target;

	sf::RenderStates skyStates{&m_shader};
	target.draw(m_skyBackground, skyStates);
	//target.draw(m_skyBackground, states);

	target.draw(m_sunMoonSprite, states);
	target.draw(m_timeText, states);
}

void DayNightCycle::updateSkyVertices() {
	//In SFML,	 the origin (0,0) is at the top left.
	//In OpenGL, the origin (0,0) is in the middle.
	//Therefore, we'll need to tweak some stuff.

	/*
	We're going to split the screen into 14 equally sized triangles
	*/

	float targetWidth  = m_target->getSize().x;
	float targetHeight = m_target->getSize().y;

	sf::Vector2f center{0, 0};
	sf::Vector2f centerLeft{-targetWidth / 2, center.y};
	sf::Vector2f centerRight{targetWidth / 2, center.y};
	sf::Vector2f centerMidLeft{-targetWidth / 4, center.y};
	sf::Vector2f centerMidRight{targetWidth / 4, center.y};

	sf::Vector2f topLeft{-targetWidth / 2, targetHeight / 2};
	sf::Vector2f topMidLeft{topLeft.x + targetWidth / 3, topLeft.y};
	sf::Vector2f topMidRight{topMidLeft.x + targetWidth / 3, topLeft.y};
	sf::Vector2f topRight{targetWidth / 2, topLeft.y};

	sf::Vector2f bottomLeft{-targetWidth / 2, -targetHeight / 2};
	sf::Vector2f bottomMidLeft{bottomLeft.x + targetWidth / 3, bottomLeft.y};
	sf::Vector2f bottomMidRight{bottomMidLeft.x + targetWidth / 3,
	                            bottomLeft.y};
	sf::Vector2f bottomRight{targetWidth / 2, bottomLeft.y};

	m_skyBackground[0].position  = topLeft;
	m_skyBackground[0].texCoords = topLeft;
	m_skyBackground[1].position  = centerMidLeft;
	m_skyBackground[1].texCoords = centerMidLeft;
	m_skyBackground[2].position  = centerLeft;
	m_skyBackground[2].texCoords = centerLeft;

	m_skyBackground[3].position  = topLeft;
	m_skyBackground[3].texCoords = topLeft;
	m_skyBackground[4].position  = topMidLeft;
	m_skyBackground[4].texCoords = topMidLeft;
	m_skyBackground[5].position  = centerMidLeft;
	m_skyBackground[5].texCoords = centerMidLeft;

	m_skyBackground[6].position  = centerMidLeft;
	m_skyBackground[6].texCoords = centerMidLeft;
	m_skyBackground[7].position  = topMidLeft;
	m_skyBackground[7].texCoords = topMidLeft;
	m_skyBackground[8].position  = center;
	m_skyBackground[8].texCoords = center;

	m_skyBackground[9].position   = topMidLeft;
	m_skyBackground[9].texCoords  = topMidLeft;
	m_skyBackground[10].position  = topMidRight;
	m_skyBackground[10].texCoords = topMidRight;
	m_skyBackground[11].position  = center;
	m_skyBackground[11].texCoords = center;

	m_skyBackground[12].position  = center;
	m_skyBackground[12].texCoords = center;
	m_skyBackground[13].position  = topMidRight;
	m_skyBackground[13].texCoords = topMidRight;
	m_skyBackground[14].position  = centerMidRight;
	m_skyBackground[14].texCoords = centerMidRight;

	m_skyBackground[15].position  = topMidRight;
	m_skyBackground[15].texCoords = topMidRight;
	m_skyBackground[16].position  = topRight;
	m_skyBackground[16].texCoords = topRight;
	m_skyBackground[17].position  = centerMidRight;
	m_skyBackground[17].texCoords = centerMidRight;

	m_skyBackground[18].position  = centerMidRight;
	m_skyBackground[18].texCoords = centerMidRight;
	m_skyBackground[19].position  = topRight;
	m_skyBackground[19].texCoords = topRight;
	m_skyBackground[20].position  = centerRight;
	m_skyBackground[20].texCoords = centerRight;

	m_skyBackground[21].position  = centerLeft;
	m_skyBackground[21].texCoords = centerLeft;
	m_skyBackground[22].position  = centerMidLeft;
	m_skyBackground[22].texCoords = centerMidLeft;
	m_skyBackground[23].position  = bottomLeft;
	m_skyBackground[23].texCoords = bottomLeft;

	m_skyBackground[24].position  = bottomLeft;
	m_skyBackground[24].texCoords = bottomLeft;
	m_skyBackground[25].position  = centerMidLeft;
	m_skyBackground[25].texCoords = centerMidLeft;
	m_skyBackground[26].position  = bottomMidLeft;
	m_skyBackground[26].texCoords = bottomMidLeft;

	m_skyBackground[27].position  = centerMidLeft;
	m_skyBackground[27].texCoords = centerMidLeft;
	m_skyBackground[28].position  = center;
	m_skyBackground[28].texCoords = center;
	m_skyBackground[29].position  = bottomMidLeft;
	m_skyBackground[29].texCoords = bottomMidLeft;

	m_skyBackground[30].position  = bottomMidLeft;
	m_skyBackground[30].texCoords = bottomMidLeft;
	m_skyBackground[31].position  = center;
	m_skyBackground[31].texCoords = center;
	m_skyBackground[32].position  = bottomMidRight;
	m_skyBackground[32].texCoords = bottomMidRight;

	m_skyBackground[33].position  = center;
	m_skyBackground[33].texCoords = center;
	m_skyBackground[34].position  = centerMidRight;
	m_skyBackground[34].texCoords = centerMidRight;
	m_skyBackground[35].position  = bottomMidRight;
	m_skyBackground[35].texCoords = bottomMidRight;

	m_skyBackground[36].position  = bottomMidRight;
	m_skyBackground[36].texCoords = bottomMidRight;
	m_skyBackground[37].position  = centerMidRight;
	m_skyBackground[37].texCoords = centerMidRight;
	m_skyBackground[38].position  = bottomRight;
	m_skyBackground[38].texCoords = bottomRight;

	m_skyBackground[39].position  = centerMidRight;
	m_skyBackground[39].texCoords = centerMidRight;
	m_skyBackground[40].position  = centerRight;
	m_skyBackground[40].texCoords = centerRight;
	m_skyBackground[41].position  = bottomRight;
	m_skyBackground[41].texCoords = bottomRight;

	/*
	Simply create a quad with vertices at the corners of the screen

	m_skyBackground[0].position = { -targetWidth / 2, targetHeight / 2 };
	m_skyBackground[0].texCoords = { -targetWidth / 2, targetHeight / 2 };
	//top right
	m_skyBackground[1].position = { targetWidth / 2, targetHeight / 2 };
	m_skyBackground[1].texCoords = { targetWidth / 2, targetHeight / 2 };
	//bottom right
	m_skyBackground[2].position = { targetWidth / 2, -targetHeight / 2 };
	m_skyBackground[2].texCoords = { targetWidth / 2, -targetHeight / 2 };
	//bottom left
	m_skyBackground[3].position = { -targetWidth / 2, -targetHeight / 2 };
	m_skyBackground[3].texCoords = { -targetWidth / 2, -targetHeight / 2 };
	*/
}
