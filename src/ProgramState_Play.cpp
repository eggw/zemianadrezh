#include "ProgramState_Play.h"

#include "LoggerNetwork.h"

ProgramState_Play::ProgramState_Play(Program& _program)
    : 	ProgramState(_program),
       m_localServer(),
       m_client(),
       m_rendererChunk(*m_program.m_window) {

    //m_serverConnection.setBlocking(false);

    m_client.m_networkManager.connect(sf::IpAddress::LocalHost, 7777);
    m_localServer.m_networkManager.accept();

    m_client.m_networkManager.sendPacket(Packet::Type::REQUEST_WORLD);

    auto worldData = m_client.getWorld().getChunks();
    for(auto& chunk : worldData){
        m_rendererChunk.update(&chunk);
    }
}

ProgramState_Play::~ProgramState_Play() {

}

void ProgramState_Play::getInput() {
    sf::Event event;

    while(m_program.m_window->pollEvent(event)) {

        if(event.type == sf::Event::Closed) {
            m_program.m_window->close();
        }

        if(event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::Escape) {
                m_program.m_window->close();
            }
        }

    }

}

void ProgramState_Play::update() {
    m_localServer.update();
    m_client.update();
}

void ProgramState_Play::draw() {
    m_rendererChunk.draw();
}
