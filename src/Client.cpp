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

	handleIncomingMessages();
	handleOutgoingMessages();

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

void Client::requestWorldChunks(){
	m_networkManager.sendPacket(Packet::Type::REQUEST_WORLD);
}

void Client::respawnPlayer() {
	m_player.setPosition({m_world.getCenter().x, 0});
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

const ChatBox* Client::getChatBox() const{
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
void Client::handleIncomingMessages(){
    std::unique_ptr<std::pair<std::string, std::string>> ptr(new std::pair<std::string, std::string>);
	if(m_networkManager.receivedMessage(ptr.get())){
		m_chatBox.appendMessage(ptr->first, ptr->second);
		m_networkManager.clearLastReceivedMessage();
	}
}

//We check to see if there is any new outgoing message from the chatbox,
//and if so, we'll set it as our latest m_pendingMessage, then send it
//to the server
void Client::handleOutgoingMessages(){
    std::unique_ptr<std::pair<std::string, std::string>> ptr(new std::pair<std::string, std::string>);
	if(m_chatBox.completedMessage(ptr.get())){
		m_pendingMessage = *ptr;
		m_networkManager.sendPacket(Packet::Type::CHAT_MESSAGE);
	}
}


/*
std::vector<std::pair<bool,std::pair<std::string, std::string>>>& Client::getPendingMessages(){
	return m_pendingMessages;
}


//See Client.h and ChatBox.h for an explanation of
//m_pendingMessages and ChatBox::getPendingMessage() respectively.
void Client::handlePendingMessages(){

	//First, we check if ChatBox has a pending message available for us
	//to insert into m_pendingMessages. We set the bool to false, since
	//it hasn't been sent over the network yet, and call ChatBox::clearPendingMessage()
	//since we don't need that there anymore.

	auto pendingMessage = m_chatBox.getPendingMessage();
	if(pendingMessage.first != "" && pendingMessage.second != ""){
        m_pendingMessages.push_back(std::make_pair(false,pendingMessage));
        m_chatBox.clearPendingMessage();
	}

    //See the CHAT_MESSAGE section of NetworkManagerClient::sendPacket() for
    //details on how the function works. It iterates through m_pendingMessages,
    //and attempts to send each one over the network. If it succeeds, it sets
    //the outer pair's boolean to true
	if(!m_pendingMessages.empty()){
        m_networkManager.sendPacket(Packet::Type::CHAT_MESSAGE);
	}

	//Finally, we'll cull all the messages - that have already been verified to
	//have been sent - from m_pendingMessages
	m_pendingMessages.erase(std::remove_if(
						m_pendingMessages.begin(),
						m_pendingMessages.end(),
						[](const std::pair<bool,std::pair<std::string, std::string>>& m){
							return m.first;
						}), m_pendingMessages.end());
}
*/
