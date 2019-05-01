#include "Server.h"

Server::Server(bool _allowConnections)
            : GameInstance(),
              m_networkManager(*this),
              m_allowConnections(_allowConnections) {
	m_world.addChunk(0, 2, false);
}

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

bool Server::connectionsAllowed() {
	return m_allowConnections;
}
