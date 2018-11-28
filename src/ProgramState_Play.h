#ifndef PROGRAMSTATE_PLAY_H_INCLUDED
#define PROGRAMSTATE_PLAY_H_INCLUDED

#include "ProgramState.h"

#include "Server.h"
#include "Client.h"
#include "RendererChunk.h"
#include "RendererDrawable.h"

class ProgramState_Play : public ProgramState {
    public:
        ProgramState_Play(Program& _program,
                          sf::IpAddress _ipAddress);
        ~ProgramState_Play();

        void getInput(sf::Event& _event);
        void update();
        void draw();

        bool clientConnected() const;

    private:
        void onResize(sf::Vector2u _newSize);

    private:
        Client m_client;
        RendererChunk m_rendererChunk;
        RendererDrawable<Player> m_rendererPlayer;
        RendererDrawable<ChatBox> m_rendererChatbox;

        sf::View m_view;

        //This function is responsible for calling RendererChunk's update
        //function on chunks that have just had their data updated
void renderUpdatedChunks();

        //This function is responsible for calling RendererPlayer's add
        //function on players that have joined but are not being rendered
        //TODO: refactor it.
        void renderNewPlayers();
};

#endif // PROGRAMSTATE_PLAY_H_INCLUDED
