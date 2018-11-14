#include "Client.h"

#include "Server.h"

#include "LoggerNetwork.h"

Client::Client(	sf::RenderWindow& _window,
                sf::IpAddress _serverIP,
                Server* _localServer)
    :GameInstance(),
     m_networkManager(*this),
     m_serverIP(_serverIP),
     m_localServer(_localServer),
     m_player(),
     m_chatBox(_window, m_player.getName()) {

    if(m_localServer != nullptr) {
        m_world = m_localServer->getWorld();
        m_otherPlayers = m_localServer->getOtherPlayers();

        LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::CLIENT,
                                          LoggerNetwork::LOG_MESSAGE::CONNECTION_LOCALHOST);
    }

}

void Client::getInput(sf::Event& _event) {
    m_player.getInput();
    m_chatBox.getInput(_event);
}

void Client::update() {
    m_player.update();

    if(m_otherPlayers != nullptr){
            for(auto& player : *m_otherPlayers){
            player->update();
        }
    }

    handleIncomingMessages();
    handleOutgoingMessages();
    m_networkManager.update();

    m_chatBox.update();

    if(!isLocal()) {
        GameInstance::update();
    }
}

void Client::sendPackets() {
    m_networkManager.sendPacket(Packet::Type::DATA_PLAYER);
}

void Client::receivePackets() {
    m_networkManager.receivePacket();
}

void Client::respawnPlayer(sf::Vector2f _pos) {
    m_player.setPosition(_pos);
}

void Client::updateOtherPlayers(Player::EncodedPlayerData _data) {
    if(m_otherPlayers == nullptr) {
        m_otherPlayers = std::shared_ptr<std::vector<std::unique_ptr<Player>>>
                         (new std::vector<std::unique_ptr<Player>>());
    }

    if(_data.playerName == m_player.getName()) {
        return;
    }

    bool found{false};

    for(auto& player : *m_otherPlayers) {
        if(player->getName() == _data.playerName) {
            player->parseData(_data);
            found = true;
        }
    }

    if(!found) {
        auto newPlayer = std::unique_ptr<Player>(new Player());
        newPlayer->parseData(_data);
        m_otherPlayers->push_back(std::move(newPlayer));
    }
}

const Player* Client::getPlayer() const {
    return &m_player;
}

const ChatBox* Client::getChatBox() const {
    return &m_chatBox;
}

bool Client::isConnected() const {
    return m_networkManager.connectionActive();
}

bool Client::isLocal() const {
    return m_localServer != nullptr;
}

std::pair<std::string, std::string> Client::getPendingMessage() const {
    return m_pendingMessage;
}

//We check to see if any new messages have been received from our
//network manager. If so, we'll add it to the chatbox and clear it from
//the network manager.
void Client::handleIncomingMessages() {
    std::unique_ptr<std::pair<std::string, std::string>> ptr(new std::pair<std::string, std::string>);
    if(m_networkManager.receivedMessage(ptr.get())) {
        m_chatBox.appendMessage(ptr->first, ptr->second);
        m_networkManager.clearLastReceivedMessage();
    }
}

//We check to see if there is any new outgoing message from the chatbox,
//and if so, we'll set it as our latest m_pendingMessage, then send it
//to the server
void Client::handleOutgoingMessages() {
    std::unique_ptr<std::pair<std::string, std::string>> ptr(new std::pair<std::string, std::string>);
    if(m_chatBox.completedMessage(ptr.get())) {
        m_pendingMessage = *ptr;
        m_networkManager.sendPacket(Packet::Type::CHAT_MESSAGE);
    }
}
