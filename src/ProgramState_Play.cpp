#include "ProgramState_Play.h"

#include "LoggerNetwork.h"

ProgramState_Play::ProgramState_Play(Program& _program)
    : 	ProgramState(_program),
       m_localServer(),
       m_client(),
       m_rendererChunk(*m_program.m_window),
       m_rendererPlayer(*m_program.m_window) {

    m_client.m_networkManager.connect(sf::IpAddress::LocalHost, 7777);
    m_localServer.m_networkManager.accept();

    m_client.m_networkManager.sendPacket(Packet::Type::REQUEST_WORLD);
    m_localServer.m_networkManager.receivePacket();
    m_client.m_networkManager.receivePacket();

    auto worldData = m_client.getWorld().getChunks();
    for(auto& chunk : worldData) {
        m_rendererChunk.update(&chunk);
    }

    m_rendererPlayer.addPlayer(m_client.getPlayer());
}

ProgramState_Play::~ProgramState_Play() {

}

void ProgramState_Play::getInput() {
    m_client.getInput();
}

void ProgramState_Play::update() {
    m_client.sendPackets();
    m_localServer.receivePackets();
    m_client.receivePackets();

    m_client.update();
    m_localServer.update();
    m_rendererPlayer.update();
}

void ProgramState_Play::draw() {
    m_rendererChunk.draw();
    m_rendererPlayer.draw();
}
