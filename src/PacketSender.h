#ifndef PACKETSENDER_H_INCLUDED
#define PACKETSENDER_H_INCLUDED

#include <memory>
#include <SFML/Network.hpp>

#include <SPSS/Util/Singleton.h>

/*
When using non-blocking TcpSockets, it is important to send
the same packet object over again if TcpSocket.send() returns
sf::Socket::Status::Partial (or if it's unable to send for \
another reason, such as not being ready). We'll use this class
as a wrapperaround all packet sending operations to ensure that
packets are being sent properly, even if it takes several frames
to do so.

See the footnote on the following page for a detailed explanation:
https://www.sfml-dev.org/tutorials/2.5/network-socket.php
*/

typedef std::shared_ptr<sf::Packet> PacketSharedPtr;
typedef std::unique_ptr<sf::Packet> PacketUPtr;

class PacketSender : public spss::Singleton<PacketSender> {
  public:
	struct TCPPacketWithSocket {
		TCPPacketWithSocket(sf::TcpSocket*        _socket,
		                    const PacketSharedPtr _packet)
		            : m_socket{_socket}, m_packet{_packet} {
		}

		sf::TcpSocket*  m_socket;
		PacketSharedPtr m_packet;
	};

	PacketSender();

	//Attempts to send the packet, and if not successful, moves
	//it along with its sending socket into m_packetData, where
	//sending the very same packet object can be attempted again
	void send(sf::TcpSocket* _socket, const PacketSharedPtr _packet);

	//Send is also overloaded to take in UdpSockets. We don't have
	//to do the whole TCP thing of reattempting to send the same
	//packet with UDP sockets.
	void send(sf::UdpSocket*        _socket,
	          const PacketSharedPtr _packet,
	          const sf::IpAddress&  _recipient,
	          unsigned short        _port);

	//Loops through m_packetData, attempts to send the packets,
	//and removes them from the vector if the attempt was successful
	void update();

  private:
	//Data members --------------------------------
	std::vector<TCPPacketWithSocket> m_packetData;
	//---------------------------------------------
};

#endif // PACKETSENDER_H_INCLUDED
