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
	 m_player(_localServer == nullptr ? "RemotePlayer" : "LocalPlayer"),
	 m_chatBox(_window, m_player.getName()) {

	if(m_localServer != nullptr) {
		m_world = m_localServer->getWorld();
		m_players = m_localServer->getPlayers();

		LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::CLIENT,
										  LoggerNetwork::LOG_MESSAGE::CONNECTION_LOCALHOST);
	}
	else{
        m_players = std::shared_ptr<std::vector<std::unique_ptr<Player>>>
					 (new std::vector<std::unique_ptr<Player>>());
	}

}

Client::~Client() {
    if(!isLocal()) {
		m_networkManager.sendPacket(Packet::TCPPacket::QUIT);
    }
}

void Client::getInput(sf::Event& _event) {
	m_player.getInput();
	m_chatBox.getInput(_event);
}

void Client::update() {
	m_player.update();

	if(m_players != nullptr) {
		for(auto& player : *m_players) {
			player->update();
		}
	}

	handleIncomingMessages();
	handleOutgoingMessages();
	m_networkManager.update();

	m_chatBox.update();
}

void Client::sendPackets() {
	m_networkManager.sendPacket(Packet::UDPPacket::DATA_PLAYER);
}

void Client::receivePackets() {
	m_networkManager.receiveUDPPackets();
	m_networkManager.receiveTCPPackets();
}

void Client::updatePlayer(Player::EncodedPlayerData _data) {
	if(_data.playerName == m_player.getName()) {
		return;
	}

	bool found{false};

	for(auto& player : *m_players) {
		if(player->getName() == _data.playerName) {
			player->parseData(_data);
			found = true;
		}
	}

	if(!found) {
		addPlayer(_data);
	}
}

void Client::addPlayer(Player::EncodedPlayerData _data) {
	if(_data.playerName == m_player.getName()) {
		return;
	}

	auto newPlayer = std::unique_ptr<Player>(new Player(_data.playerName));
	newPlayer->parseData(_data);
	m_players->push_back(std::move(newPlayer));
}

void Client::respawnPlayer() {
    m_player.setPosition({m_world.getCenter().x, 0});
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
		m_networkManager.sendPacket(Packet::TCPPacket::CHAT_MESSAGE);
	}
}
