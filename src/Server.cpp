#include "Server.h"

Server::Server(bool _allowConnections)
            : GameInstance(),
              m_networkManager(*this),
              m_allowConnections(_allowConnections) {
	initialise();
	m_world->initialise();
}

//Automatically send out a packet indicating the server has been closed
//upon its destruction
Server::~Server() {
	m_networkManager.sendPacket(Packet::TCP::CONNECTIONLOST);
}

void Server::receivePackets() {
	m_networkManager.receiveUDPPackets();
	m_networkManager.receiveTCPPackets();
}

void Server::acceptConnections() {
	m_networkManager.accept();
}

void Server::update(int _timeslice) {
	m_world->tickTime(_timeslice);
	m_networkManager.update();
}

bool Server::connectionsAllowed() const {
	return m_allowConnections;
}

size_t Server::connectedPlayers() const {
	return m_networkManager.connectedPlayers();
}
