#include "DayNightCycle.h"

#include <math.h>

#include "TextureManager.h"
#include "FontManager.h"
#include "WorldTime.h"

constexpr int DAY_BEGIN_HOUR{5};
constexpr int DAY_END_HOUR{19};

constexpr int NIGHT_BEGIN_HOUR{19};
constexpr int NIGHT_END_HOUR{5};

//This value controls the vertical position of the horizon.
//For example, if set to 0.5, the horizon will start at the
//vertical center of the screen.
constexpr float HORIZON_AS_HEIGHT_RATIO{0.65};

DayNightCycle::DayNightCycle()
            : m_sunMoonSprite{},
              m_skyBackground{sf::PrimitiveType::Triangles, 42},
              m_timeText{},
              m_target{nullptr},
              m_shader{},
              m_lastTargetSize{},
              m_planetYProgress{0} {
	m_timeText.setFont(
	  FontManager::get_instance().getFont(FontManager::TYPE::ANDY));
	m_timeText.setCharacterSize(30);
	m_timeText.setOutlineThickness(1);
	m_timeText.setOrigin(m_timeText.getGlobalBounds().width / 2,
	                     m_timeText.getGlobalBounds().height / 2);

	if (!m_shader.loadFromFile("assets/shaders/cycle_vertex.glsl",
	                           "assets/shaders/cycle_fragment.glsl")) {
		//TODO: error handling?
	}

	m_shader.setUniform("sky",
	                    TextureManager::get_instance().getTexture(
	                      TextureManager::TYPE::CYCLE_SKY_GRADIENT));
	m_shader.setUniform("glow",
	                    TextureManager::get_instance().getTexture(
	                      TextureManager::TYPE::CYCLE_GLOW_GRADIENT));
}

void DayNightCycle::update(const WorldTime& _time) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma)) {
		_time.pause();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period)) {
		_time.unpause();
	}
	//A draw call needs to be attempted at least once to set our target
	//before updating can begin
	if (m_target == nullptr) {
		return;
	}

	updateSkyVertices();
	updateTimeText(_time);
	updatePlanetTexture(_time);
	updatePlanetPosition(_time);
	sendUniformsToShader(_time);
}

void DayNightCycle::draw(sf::RenderTarget& target,
                         sf::RenderStates  states) const {
	m_target = &target;

	sf::RenderStates skyStates{&m_shader};
	target.draw(m_skyBackground, skyStates);
	target.draw(m_sunMoonSprite, states);
	target.draw(m_timeText, states);
}

void DayNightCycle::updateSkyVertices() {
	//In SFML,	 the origin (0,0) is at the top left.
	//In OpenGL, the origin (0,0) is in the middle.
	//
	//We're going to split the screen into 14 equally sized triangles
	//
	//Also, we're going to skip this whole process if the target size
	//hasn't changed since the last time we called updateSkyVertices()

	float targetWidth  = m_target->getSize().x;
	float targetHeight = m_target->getSize().y;

	if (m_lastTargetSize.x == targetWidth &&
	    m_lastTargetSize.y == targetHeight) {
		return;
	}

	m_lastTargetSize.x = targetWidth;
	m_lastTargetSize.y = targetHeight;

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
}

void DayNightCycle::updateTimeText(const WorldTime& _time) {
	auto str{_time.getString()};
	m_timeText.setString(str);
	m_timeText.setPosition(
	  m_target->getSize().x - 1.2 * m_timeText.getGlobalBounds().width, 0);
}

void DayNightCycle::updatePlanetTexture(const WorldTime& _time) {
	if (isDaytime(_time)) {
		m_sunMoonSprite.setTexture(
		  TextureManager::get_instance().getTexture(TextureManager::TYPE::SUN));
	}
	else {
		m_sunMoonSprite.setTexture(TextureManager::get_instance().getTexture(
		  TextureManager::TYPE::MOON));
	}
	//We're going to set the origin to be at the rightmost point horizontally and the
	//uppermost point vertically. Later, when determining xPos, we add the width of the
	//sprite to our calculation. This is to ensure a seamless transition (at the exact
	//moment the sun is out of our view, the moon starts to come in, and vice versa)
	m_sunMoonSprite.setOrigin(m_sunMoonSprite.getGlobalBounds().width, 0);
}

void DayNightCycle::updatePlanetPosition(const WorldTime& _time) {
	static const double pi{std::atan(1) * 4};
	auto                hhmm{getWrappedTime(_time)};
	auto                hourRange{getCurrentHourRange(_time)};
	//Get the "percentage" of how far along the X axis the sun should be (for example, at 12, it should be
	//exactly 50% of the way.
	//First, calculate the difference between the hours.
	//[For example, from 6 to 18, this would be 12]
	int difference{abs(hourRange.first - hourRange.second)};
	//Then, calculate the offset from the begin hour.
	//[For example, if we take 12, 12 - 6 = 6]
	int offset{abs(hourRange.first - hhmm.hours)};
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
	//from the east to the west and not vice versa.
	//Also, since the sprite's X origin is at its rightmost point, we'll add the sprite's width to
	//the view size in our xPos calculation.

	double xPos{
	  (1 - progressInPercent) *
	  (m_target->getSize().x + m_sunMoonSprite.getGlobalBounds().width)};

	//We're also going to invert the sin result in yPos so the movement will be
	//down->up->down, rather than up->down->up.

	m_planetYProgress = sin(progressInPercent * pi);

	double yPos{(1 - m_planetYProgress) * m_target->getSize().y *
	            HORIZON_AS_HEIGHT_RATIO};

	m_sunMoonSprite.setPosition(xPos, yPos);
}

void DayNightCycle::sendUniformsToShader(const WorldTime& _time) {
	//We're going to send the position of the sun/moon sprite
	//as the center. Since the sprite's origins are (width, 0),
	//we'll have to adjust the position values appropriately.
	sf::Vector2f openGLCoordinates{m_sunMoonSprite.getPosition()};

	//First, since the sprite's x origin is at its rightmost point, we'll
	//subtract half its width from the x coordinate.
	//
	//Also, since the sprite's y origin is at its uppermost point, we'll
	//add half its height to the y coordinate.
	openGLCoordinates.x -= m_sunMoonSprite.getGlobalBounds().width / 2;
	openGLCoordinates.y += m_sunMoonSprite.getGlobalBounds().height / 2;

	//We'll then get the positions as ratios of the current window size
	openGLCoordinates.x /= m_target->getSize().x;
	openGLCoordinates.y /= m_target->getSize().y;

	//In SFML,   upperleft = (0, 0)  and lowerright = (1, 1)
	//(center is 0.5, 0.5)
	//
	//In OpenGL, upperleft = (-1, 1) and lowerright = (1, -1)
	//(center is 0, 0)
	//
	//We're going to convert both x and y positions from the
	//(0, 1) range of SFML to the (-1, 1) range of OpenGL.
	//
	//We use the following simple formula to achieve this:
	//
	//OldRange = (OldMax - OldMin)
	//NewRange = (NewMax - NewMin)
	//NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin

	openGLCoordinates.x = (openGLCoordinates.x * 2) - 1;
	openGLCoordinates.y = (((openGLCoordinates.y - 1) * 2) / -1) - 1;

	m_shader.setUniform(
	  "planetPosition",
	  sf::Vector3f{openGLCoordinates.x, openGLCoordinates.y, 0.f});
	m_shader.setUniform("planetYProgress", m_planetYProgress);
	m_shader.setUniform("daytime", isDaytime(_time));
}

bool DayNightCycle::isDaytime(const WorldTime& _time) const {
	auto hhmm{_time.get()};
	if (hhmm.hours >= DAY_BEGIN_HOUR && hhmm.hours < DAY_END_HOUR) {
		return true;
	}
	return false;
}

std::pair<int, int> DayNightCycle::getCurrentHourRange(
  const WorldTime& _time) const {
	std::pair<int, int> result{};
	if (isDaytime(_time)) {
		result = std::make_pair(DAY_BEGIN_HOUR, DAY_END_HOUR);
	}
	else {
		result = std::make_pair(NIGHT_BEGIN_HOUR, NIGHT_END_HOUR);
	}

	//These two if statements make sure that everything runs smoothly in cases such as the following:
	//HOUR_START = 19, HOUR_END = 5
	//In such a case, the algorithm will get confused when we're between 0 and 4 as 0 is not greater
	//than 19. Adding 24 to the end hour, in addition to doing the same for 0 (see getWrappedTime())
	//will fix the logic and account for 24h wrapping.
	if (result.first > result.second) {
		result.second += 24;
	}

	return result;
}

//See getCurrentHourRange() for a more detailed comment about why we may need to add 24.
HHMM DayNightCycle::getWrappedTime(const WorldTime& _time) const {
	auto hhmm{_time.get()};

	if (hhmm.hours < getCurrentHourRange(_time).first) {
		hhmm.hours += 24;
	}

	return hhmm;
}
