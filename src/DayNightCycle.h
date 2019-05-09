#ifndef DAYNIGHTCYCLE_H_INCLUDED
#define DAYNIGHTCYCLE_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "World.h"

/*
This class handles everything to do with the VISUAL effects of the day-night cycle, including:

-The sun/moon's position
-Shaders

Just give it a world, and it will handle everything else. It inherits from sf::Drawable so as
to decouple the aesthetic portions of the day-night cycle from everything else. Simply draw it.

Saving this for future reference:
https://csc.lsu.edu/~kooima/misc/cs594/final/part2.html
*/

class DayNightCycle : public sf::Drawable {
  public:
	DayNightCycle(const World& _world);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	//This function will deal with managing either the sun or the moon's
	//sprite and position, depending on the world time
	void update();

  private:
  	//Functions -----------------------------------
  	void updateSkyVertices();
  	//getInput() is a temporary function that detects
	//if comma or period is pressed and pauses/resumes
	//the time as necessary. Just for testing for now.
	void getInput();
	void updateTimeText();
	void updatePlanetTexture();
	void updatePlanetPosition();
	void sendUniformsToShader();
	bool isDaytime() const;
	//If it's currently daytime, gets the values of
	//the begin and end daytime hours. Otherwise, gets
	//the values for nighttime.
	std::pair<int, int> getCurrentHourRange() const;
	//Simply gets the current time, but also performs an
	//additional check to see if any additional 24h
	//wrapping is necessary
	HHMM getWrappedTime() const;
	//---------------------------------------------

	//Data members --------------------------------
	const World& m_world;
	sf::Sprite   m_sunMoonSprite;
	//m_skyBackground will contain the vertices for all 14 triangles
	//that the screen is divided into. Note that four vertices at the
	//edges of the screen would also suffice, but let's keep the 14
	//triangles for now.

	//Shaders will determine the appropriate gradients depending on the
	//time of day and position of the sun in the sky.
	sf::VertexArray m_skyBackground;
	sf::Text        m_timeText;
	//In order to avoid having a lot of extra code to handle resizing, we'll
	//simply update m_target to have it point to the RenderTarget passed into
	//the draw function.
	mutable sf::RenderTarget* m_target;
	mutable sf::Shader        m_shader;
	//---------------------------------------------
};

#endif // DAYNIGHTCYCLE_H_INCLUDED
