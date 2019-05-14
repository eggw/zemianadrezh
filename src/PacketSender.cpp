#include "PacketSender.h"

PacketSender::PacketSender() {
}

void PacketSender::send(sf::TcpSocket* _socket, const PacketSharedPtr _packet) {
	TCPPacketWithSocket data{_socket, _packet};

	auto status = data.m_socket->send(*(data.m_packet));
	if (status != sf::Socket::Status::Done) {
		m_packetData.push_back(data);
	}
}

void PacketSender::send(sf::UdpSocket*        _socket,
                        const PacketSharedPtr _packet,
                        const sf::IpAddress&  _recipient,
                        unsigned short        _port) {
	_socket->send(*_packet, _recipient, _port);
}

void PacketSender::update() {
	m_packetData.erase(
	  std::remove_if(m_packetData.begin(),
	                 m_packetData.end(),
	                 [](const TCPPacketWithSocket& p) {
		                 //return true to have it removed
		                 auto status = p.m_socket->send(*(p.m_packet));
		                 return (status == sf::Socket::Status::Done);
	                 }),
	  m_packetData.end());
}
