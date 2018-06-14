#include "Client.h"

#include "Server.h"

#include "LoggerNetwork.h"

Client::Client(Server* _localServer)
    :GameInstance(),
     m_networkManager(*this),
     m_localServer(_localServer) {

     	if(m_localServer != nullptr){
            m_world = m_localServer->getWorld();

            LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::CLIENT,
                                          LoggerNetwork::LOG_MESSAGE::CONNECTION_LOCALHOST);
     	}

}

void Client::update(){
    m_networkManager.receivePacket();
}
