#include "Server.h"
#include <iostream>

Server::Server()
	:	m_socket(),
		m_world(){

	sf::Socket::Status status = m_socket.connect(sf::IpAddress::LocalHost, 7777);
	if (status != sf::Socket::Done){
		std::cout << "Error!" << std::endl;
	}
	else{
		std::cout << "Success!" << std::endl;
	}
}


const World& Server::getWorld() const{
	return m_world;
}
