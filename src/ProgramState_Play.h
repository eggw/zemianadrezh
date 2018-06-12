#ifndef PROGRAMSTATE_PLAY_H_INCLUDED
#define PROGRAMSTATE_PLAY_H_INCLUDED

#include "ProgramState.h"

#include "Server.h"
#include "RendererChunk.h"

class ProgramState_Play : public ProgramState {
  public:
    ProgramState_Play(Program& _program);
    ~ProgramState_Play();

	void getInput();
    void update();
    void draw();

  private:
	Server m_server;
	RendererChunk m_rendererChunk;
};

#endif // PROGRAMSTATE_PLAY_H_INCLUDED
