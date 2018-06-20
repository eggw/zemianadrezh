#ifndef PROGRAMSTATE_PLAY_H_INCLUDED
#define PROGRAMSTATE_PLAY_H_INCLUDED

#include "ProgramState.h"

#include "Server.h"
#include "Client.h"
#include "RendererChunk.h"
#include "RendererPlayer.h"
#include "ChatBox.h"

class ProgramState_Play : public ProgramState {
	public:
		ProgramState_Play(Program& _program);
		~ProgramState_Play();

		void getInput(sf::Event& _event);
		void update();
		void draw();

	private:
		Server m_localServer;
		Client m_client;
		RendererChunk m_rendererChunk;
		RendererPlayer m_rendererPlayer;
		ChatBox m_chatBox;
};

#endif // PROGRAMSTATE_PLAY_H_INCLUDED
