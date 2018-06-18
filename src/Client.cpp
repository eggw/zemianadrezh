#include "Client.h"

#include "Server.h"

#include "LoggerNetwork.h"

Client::Client(Server* _localServer)
    :GameInstance(),
     m_networkManager(*this),
     m_localServer(_localServer),
     m_player(){

    if(m_localServer != nullptr) {
        m_world = m_localServer->getWorld();

        LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::CLIENT,
                                          LoggerNetwork::LOG_MESSAGE::CONNECTION_LOCALHOST);
    }

    //m_player.setPosition(m_world.getCenter());

}

void Client::getInput() {
    m_player.getInput();
}

void Client::update() {
    m_player.update();
    m_networkManager.receivePacket();
}

const Player* Client::getPlayer() const{
    return &m_player;
}

bool Client::isLocal() const {
    return m_localServer != nullptr;
}
