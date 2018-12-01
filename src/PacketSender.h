#ifndef PACKETSENDER_H_INCLUDED
#define PACKETSENDER_H_INCLUDED

#include <memory>
#include <SFML/Network.hpp>

#include "Singleton.h"

/*
When using non-blocking TcpSockets, it is important to send
the same packet object over again if TcpSocket.send() returns
sf::Socket::Status::Partial. We'll use this class as a wrapper
around all packet sending operations to ensure that packets
are being sent properly, even if it takes several frames to
do so.

See the footnote on the following page for a detailed explanation:
https://www.sfml-dev.org/tutorials/2.5/network-socket.php
*/

typedef std::shared_ptr<sf::Packet> PacketSharedPtr;
typedef std::unique_ptr<sf::Packet> PacketUPtr;

class PacketSender : public Singleton<PacketSender> {
	public:
		struct TCPPacket {
			TCPPacket(sf::TcpSocket* _socket, PacketSharedPtr _packet)
				:   m_socket{_socket},
					m_packet{_packet} {}

			sf::TcpSocket* m_socket;
			PacketSharedPtr m_packet;
		};

		PacketSender();

		void send(sf::TcpSocket* _socket, PacketSharedPtr _packet);
		void send(sf::UdpSocket* _socket, PacketSharedPtr _packet, sf::IpAddress _recipient, unsigned short _port);
		void update();

	private:
		std::vector<TCPPacket> m_packetData;
};


#endif // PACKETSENDER_H_INCLUDED
