#ifndef PROGRAMSTATE_PLAY_H_INCLUDED
#define PROGRAMSTATE_PLAY_H_INCLUDED

#include "ProgramState.h"

#include "Server.h"
#include "Client.h"
#include "RendererChunk.h"
#include "RendererDrawable.h"
#include "RendererSharedDrawable.h"
#include "DayNightCycle.h"

#include "Systems/SystemAnimation.h"
#include "Systems/SystemDrawing.h"
#include "Systems/SystemPhysics.h"
#include "Systems/SystemPlayerMovement.h"

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
	Client m_client;

	//We'll handle the drawing of non-entity elements with special
	//renderers. This isn't really all that necessary, though.
	RendererChunk                   m_rendererChunk;
	RendererDrawable<ChatBox>       m_rendererChatbox;
	RendererDrawable<UserInterface> m_rendererUserInterface;
	RendererDrawable<DayNightCycle> m_rendererDayNightCycle;

	SystemAnimation      m_systemAnimation;
	SystemDrawing        m_systemDrawing;
	SystemPhysics        m_systemPhysics;
	SystemPlayerMovement m_systemPlayerMovement;

	DayNightCycle m_dayNightCycle;

	//m_view is the view used to draw chunks, players, items, etc.
	sf::View m_view;
	//m_skyView is the view used to draw the background, sun/moon, etc.
	sf::View m_skyView;

	//This function is responsible for calling RendererChunk's update
	//function on chunks that have just had their data updated
	void renderUpdatedChunks();
};

#endif // PROGRAMSTATE_PLAY_H_INCLUDED
