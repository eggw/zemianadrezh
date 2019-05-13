#ifndef PROGRAMSTATE_PLAY_H_INCLUDED
#define PROGRAMSTATE_PLAY_H_INCLUDED

#include "ProgramState.h"

#include "Server.h"
#include "Client.h"
#include "RendererChunk.h"
#include "DayNightCycle.h"

class ProgramState_Play : public ProgramState {
  public:
	ProgramState_Play(Program& _program, sf::IpAddress _ipAddress);
	~ProgramState_Play();

	void getInput(sf::Event& _event);
	void update(int _timeslice);
	void draw();

	bool clientConnected() const;

  private:
	void onResize(sf::Vector2u _newSize);

  private:
	//Functions -----------------------------------
	//This function is responsible for calling RendererChunk's update
	//function on chunks that have just had their data updated (when
	//the network manager indicates that chunk data has been sent)
	void renderUpdatedChunks();
	//---------------------------------------------

	//Data members --------------------------------
	Client m_client;

	//We'll handle the drawing of non-entity elements with special
	//renderers.
	RendererChunk m_rendererChunk;
	DayNightCycle m_dayNightCycle;

	//m_view is the view used to draw chunks, players, items, etc.
	sf::View m_view;
	//m_skyView is the view used to draw the background, sun/moon, etc.
	sf::View m_skyView;
	//---------------------------------------------
};

#endif // PROGRAMSTATE_PLAY_H_INCLUDED
