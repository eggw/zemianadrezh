#ifndef PROGRAMSTATE_PLAY_H_INCLUDED
#define PROGRAMSTATE_PLAY_H_INCLUDED

#include "ProgramState.h"

#include "Server.h"
#include "Client.h"
#include "RendererChunk.h"
#include "RendererDrawable.h"
#include "RendererSharedDrawable.h"
#include "DayNightCycle.h"

class ProgramState_Play : public ProgramState {
    public:
        ProgramState_Play(Program& _program,
                          sf::IpAddress _ipAddress);
        ~ProgramState_Play();

        void getInput(sf::Event& _event);
        void update(int _timeslice);
        void draw();

        bool clientConnected() const;

    private:
        void onResize(sf::Vector2u _newSize);

    private:
        Client m_client;
        RendererChunk m_rendererChunk;
        RendererDrawable<Player> m_rendererClientPlayer;
        RendererDrawable<ChatBox> m_rendererChatbox;
        RendererDrawable<UserInterface> m_rendererUserInterface;
        RendererDrawable<DayNightCycle> m_rendererDayNightCycle;
        DayNightCycle m_dayNightCycle;

        //The client's player and the chatbox will always be drawn.
        //However, other players come and go. m_rendererPeers will use
        //weak_ptrs (to GameInstance::m_players) to draw them. When a player
        //is removed from m_players (via GameInstance::removePlayer()), and
        //thus goes out of scope, it will be detected by the weak_ptrs in
        //weak_ptr.expired(), and we can then remove it from m_objects, avoiding
        //a dangling pointer.
        RendererSharedDrawable<Player> m_rendererOtherPlayers;

        //m_view is the view used to draw chunks, players, items, etc.
        sf::View m_view;
        //m_skyView is the view used to draw the background, sun/moon, etc.
        sf::View m_skyView;

        //This function is responsible for calling RendererChunk's update
        //function on chunks that have just had their data updated
        void renderUpdatedChunks();

        //This function is responsible for calling RendererPlayer's add
        //function on players that have joined but are not being rendered
        //TODO: refactor it.
        void renderNewPlayers();

};

#endif // PROGRAMSTATE_PLAY_H_INCLUDED
