#ifndef PROGRAMSTATE_PLAY_H_INCLUDED
#define PROGRAMSTATE_PLAY_H_INCLUDED

#include "ProgramState.h"

#include "Server.h"
#include "Client.h"
#include "RendererChunk.h"
#include "RendererDrawable.h"
#include "ChatBox.h"

class ProgramState_Play : public ProgramState {
	public:
		ProgramState_Play(Program& _program);
		~ProgramState_Play();

		void getInput(sf::Event& _event);
		void update();
		void draw();

    private:
        void onResize(sf::Vector2u _newSize);

	private:
		Server m_localServer;
		Client m_client;
		ChatBox m_chatBox;
		RendererChunk m_rendererChunk;
		RendererDrawable<Player> m_rendererPlayer;
		RendererDrawable<ChatBox> m_rendererChatbox;

		sf::View m_view;
};

#endif // PROGRAMSTATE_PLAY_H_INCLUDED
