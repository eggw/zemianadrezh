#ifndef PROGRAMSTATE_PLAY_H_INCLUDED
#define PROGRAMSTATE_PLAY_H_INCLUDED

#include "ProgramState.h"

#include "World.h"
#include "RendererChunk.h"

class ProgramState_Play : public ProgramState {
  public:
    ProgramState_Play(Program& _program);
    ~ProgramState_Play();

	void getInput();
    void update();
    void draw();

  private:
	World m_world;
	RendererChunk m_rendererChunk;
};

#endif // PROGRAMSTATE_PLAY_H_INCLUDED
