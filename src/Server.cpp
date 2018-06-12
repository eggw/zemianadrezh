#include "Server.h"
#include <iostream>

Server::Server()
	:m_listener(),
	 m_world() {

}


const World& Server::getWorld() const {
	return m_world;
}

void Server::listen() {
	if (m_listener.listen(7777) != sf::Socket::Done) {
		std::cout << "Error listening to port" << std::endl;
	}
}

void Server::accept() {
	sf::Socket::Status status = m_listener.accept(m_clientConnection);
	if (status != sf::Socket::Done) {
		std::cout << "Error!" << std::endl;
	} else {
		std::cout << "SERVER: Success!" << std::endl;
		sf::Packet worldPacket = m_world.sendData();
		m_clientConnection.send(worldPacket);
	}
}
