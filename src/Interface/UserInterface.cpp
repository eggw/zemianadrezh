#include "Interface/UserInterface.h"

#include "Keybinds.h"

#include "NetworkManagerClient.h"

UserInterface::UserInterface(sf::RenderWindow&     _window,
                             NetworkManagerClient& _n,
                             std::string           _name)
            : m_window{_window},
              m_networkManager{_n},
              m_chatBox(_window, _name),
              m_hotbarInterface(_window) {
}

void UserInterface::getInput(sf::Event& _event) {
	m_chatBox.getInput(_event);
	m_hotbarInterface.getInput(_event);
}

void UserInterface::update(int _timeslice) {
	handleIncomingMessages();
	handleOutgoingMessages();

	m_chatBox.update();
	m_hotbarInterface.update(_timeslice);
}

void UserInterface::draw(sf::RenderStates states) const {
	m_window.draw(m_chatBox, states);
	m_window.draw(m_hotbarInterface, states);
}

//We check to see if any new messages have been received from our
//network manager. If so, we'll add it to the chatbox and clear it from
//the network manager.
void UserInterface::handleIncomingMessages() {
	auto ptr(std::make_unique<std::pair<std::string, std::string>>());

	if (m_networkManager.receivedMessage(ptr.get())) {
		m_chatBox.appendMessage(ptr->first, ptr->second);
		m_networkManager.clearLastReceivedMessage();
	}
}

//We check to see if there is any new outgoing message from the chatbox,
//and if so, we'll set it as our latest pending message, and order the
//server to send a message packet.
void UserInterface::handleOutgoingMessages() {
	auto ptr(std::make_unique<std::pair<std::string, std::string>>());

	if (m_chatBox.completedMessage(ptr.get())) {
		m_networkManager.setMessageToSend(*ptr);
		m_networkManager.sendPacket(Packet::TCPPacket::CHAT_MESSAGE);
	}
}
