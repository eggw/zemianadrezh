#ifndef PROGRAMSTATE_SPPLAY_H_INCLUDED
#define PROGRAMSTATE_SPPLAY_H_INCLUDED

#include "ProgramState.h"

#include "Server.h"
#include "Client.h"
#include "RendererChunk.h"
#include "RendererDrawable.h"

class ProgramState_SPPlay : public ProgramState {
	public:
		ProgramState_SPPlay(Program& _program);
		~ProgramState_SPPlay();

		void getInput(sf::Event& _event);
		void update();
		void draw();

    private:
        void onResize(sf::Vector2u _newSize);

	private:
		Server m_localServer;
		Client m_client;
		RendererChunk m_rendererChunk;
		RendererDrawable<Player> m_rendererPlayer;
		RendererDrawable<ChatBox> m_rendererChatbox;

		sf::View m_view;
};

#endif // PROGRAMSTATE_SPPLAY_H_INCLUDED
