#ifndef PROGRAMSTATE_PLAY_H_INCLUDED
#define PROGRAMSTATE_PLAY_H_INCLUDED

#include "ProgramState.h"

#include "Server.h"
#include "Client.h"

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
	//Data members --------------------------------
	Client m_client;
	//---------------------------------------------
};

#endif // PROGRAMSTATE_PLAY_H_INCLUDED
