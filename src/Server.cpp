#include "Server.h"

Server::Server(bool _allowConnections)
            : GameInstance(),
              m_networkManager(*this),
              m_allowConnections(_allowConnections) {
	initialise();
	m_world->addChunk(0, 2, false);
}

//Automatically send out a packet indicating the server has been closed
//upon its destruction
Server::~Server() {
	m_networkManager.sendPacket(Packet::TCPPacket::CONNECTIONLOST);
}

void Server::receivePackets() {
	m_networkManager.receiveUDPPackets();
	m_networkManager.receiveTCPPackets();
}

void Server::update(int _timeslice) {
	//TODO: figure out how to keep server/client world times synced
	//m_world.update(_timeslice);
	m_networkManager.accept();
	m_networkManager.update();
}

bool Server::connectionsAllowed() const {
	return m_allowConnections;
}
