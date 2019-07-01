#ifndef PROGRAMSTATE_PLAY_H_INCLUDED
#define PROGRAMSTATE_PLAY_H_INCLUDED

#include <SPSS/System/State.h>

#include "Program.h"
#include "Server.h"
#include "Client.h"

class ProgramState_Play : public spss::State {
  public:
	ProgramState_Play(Program& _program, sf::IpAddress _ipAddress, Server* _localServer = nullptr);
	~ProgramState_Play();

	void getInput(sf::Event& _event);
	void update(int _timeslice);
	void draw();

	bool clientConnected() const;

  private:
	void onResize(sf::Vector2u _newSize);

  private:
	//Data members --------------------------------
	Client   m_client;
	Program& m_program;
	//---------------------------------------------
};

#endif // PROGRAMSTATE_PLAY_H_INCLUDED
