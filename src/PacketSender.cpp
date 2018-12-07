#include "PacketSender.h"


PacketSender::PacketSender() {

}

//Attempts to send the packet, and if not successful, moves
//it along with its sending socket into m_packetData, where
//sending the very same packet object can be attempted again
void PacketSender::send(sf::TcpSocket* _socket, const PacketSharedPtr _packet) {
	TCPPacket data{_socket, _packet};

	auto status = data.m_socket->send(*(data.m_packet));
	if(status == sf::Socket::Status::Partial) {
		m_packetData.push_back(data);
	}
}

//Send is also overloaded to take in UdpSockets. We don't have
//to do the whole TCP thing of reattempting to send the same
//packet with UDP sockets.
void PacketSender::send(sf::UdpSocket* _socket,
						const PacketSharedPtr _packet,
						const sf::IpAddress& _recipient,
						unsigned short _port) {

	_socket->send(*_packet, _recipient, _port);
}

//Loops through m_packetData, attempts to send the packets,
//and removes them from the vector if the attempt was successful
void PacketSender::update() {
	m_packetData.erase(std::remove_if(
						   m_packetData.begin(),
						   m_packetData.end(),
	[](const TCPPacket& p) {
		//return true to have it removed
		auto status = p.m_socket->send(*(p.m_packet));
		return (status == sf::Socket::Status::Done);
	}), m_packetData.end());
}
