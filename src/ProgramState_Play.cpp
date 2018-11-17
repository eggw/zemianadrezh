#include "ProgramState_Play.h"

#include "Keybinds.h"
#include "LoggerNetwork.h"

#include <iostream>
ProgramState_Play::ProgramState_Play(Program& _program,
                                     sf::IpAddress _ipAddress)
    : 	ProgramState(_program),
       //set the last argument of m_client's constructor
       //to nullptr to test a non-local (non-resource sharing)
       //instance of Server and Client
       m_client(*m_program.m_window,
                _ipAddress,
                m_program.getServer()),
       m_rendererChunk(*m_program.m_window),
       m_rendererPlayer(*m_program.m_window),
       m_rendererChatbox(*m_program.m_window),
       m_view{sf::FloatRect(0, 0,
                            float(m_program.m_window->getSize().x),
                            float(m_program.m_window->getSize().y))} {

    /*
    The reason the client calls connect() twice is because of the way
    SFML connection statuses work: the client won't return
    sf::Socket::Done unless it's called AFTER a connection with the server
    has been estalished.

    Therefore, we'll call connect the first time in order for the log entry
    to register "awaiting response...", then a second time after the
    connection is established for "connection established!"

    See: https://en.sfml-dev.org/forums/index.php?topic=7118.0
    */

    //if(m_client.isLocal()){
        m_client.m_networkManager.connect(_ipAddress, 7777);
        m_client.m_networkManager.connect(_ipAddress, 7777);
    //}

    m_rendererChatbox.addObject(m_client.getChatBox());
    m_rendererPlayer.addObject(m_client.getPlayer());

}

ProgramState_Play::~ProgramState_Play() {

}

void ProgramState_Play::getInput(sf::Event& _event) {
    ProgramState::getInput(_event);

    if(_event.type == sf::Event::KeyPressed) {
        if(_event.key.code == Key::PAUSE_KEY) {
            m_program.pushState_Pause();
            return;
        }
    }

    m_client.getInput(_event);
}

void ProgramState_Play::update() {
    if(!m_client.isConnected()) {
        std::cout << "Not connected!"  << std::endl;
        return;
    }

    m_view.setCenter(m_client.getPlayer()->getPosition());

    m_client.receivePackets();

    updateNewChunks();
    updateNewPlayers();

    m_client.update();
    m_client.sendPackets();
}

void ProgramState_Play::draw() {
    m_program.m_window->setView(m_view);
    m_rendererPlayer.draw();
    m_rendererChunk.draw();
    m_rendererChatbox.draw();
}

void ProgramState_Play::onResize(sf::Vector2u _newSize) {
    ProgramState::onResize(_newSize);
    sf::Vector2f newSizeF{float(_newSize.x), float(_newSize.y)};
    m_view.setSize(newSizeF);
}

void ProgramState_Play::updateNewChunks() {
    std::unique_ptr<std::vector<int>> ptr(new std::vector<int>);

    if(m_client.m_networkManager.chunkDataReceived(ptr.get())) {
        auto worldData = m_client.getWorld().getChunks();
        for(auto& chunk : worldData) {
            auto result = std::find(std::begin(*ptr),
                                    std::end(*ptr),
                                    chunk.getID());

            if(result != std::end(*ptr)) {
                m_rendererChunk.update(&chunk);
            }
        }

        m_client.m_networkManager.setChunkDataProcessed(true);
    }
}

//TODO: refactor this shit
void ProgramState_Play::updateNewPlayers() {
    for(auto& player : *m_client.getOtherPlayers()) {
        m_rendererPlayer.addObject(player.get());
    }
}
