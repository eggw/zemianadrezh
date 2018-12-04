#ifndef NETWORKMANAGERSERVER_H_INCLUDED
#define NETWORKMANAGERSERVER_H_INCLUDED

#include <memory>
#include <map>

#include "SFML/Network.hpp"

#include "PacketTypes.h"
#include "PacketSender.h"

class Server;

class NetworkManagerServer {
	public:
		NetworkManagerServer(Server& _server);

		void sendPacket(Packet::TCPPacket _type,
						sf::TcpSocket* _recipient = nullptr,
						bool _exclude = false);

		void sendPacket(Packet::UDPPacket _type,
						sf::IpAddress _recipient = sf::IpAddress::None,
						bool _exclude = false);

		void receiveTCPPackets();
		void receiveUDPPackets();

		//Sends a message to all connected players
		void sendMessage(std::string _message, std::string _sender);

		void listen();
		void accept();
		void update();
	private:
		//Since the clients that connect to us via UDP will have varying
		//ports, we'll use IPInfo to conveniently store it alongside their IP
		struct IPInfo {
			IPInfo(	std::string _n,
					sf::TcpSocket& _t,
					unsigned short _p) :
				playerName{ _n },
				ipAddress{ _t.getRemoteAddress() },
				port{ _p }{};

			std::string playerName;
			sf::IpAddress ipAddress;
			unsigned short port;
		};

		//---------------------------------------------------------------------
		//Generally, our recipients are either going to be:
		//one specific recipient,
		//all recipients,
		//or all but one recipient.
		//
		//These functions take in a target recipient, defaulting to nullptr,
		//and a boolean, defaulting to false.
		//
		//If the recipient is null, they will return all possible recipients.
		//If _exclude is false, they will only return the recipient specified.
		//If _exclude is true, they will return all recipients except the one
		//specified.
		//
		//These functions are used in the sendPacket() functions.
		//---------------------------------------------------------------------

		std::vector<sf::TcpSocket*> getTCPRecipients(sf::TcpSocket* _recipient = nullptr,
													 bool _exclude = false);

		std::vector<IPInfo> 		getUDPRecipients(sf::IpAddress _recipient = sf::IpAddress::None,
													 bool _exclude = false);
		//---------------------------------------------------------------------

		//This is used to remove the matching TcpSocket from m_clientConnections
		//as well as the associated IPInfo in m_clientIPs
		//---------------------------------------------------------------------
		void removeConnection(const sf::TcpSocket* _con);
		//---------------------------------------------------------------------
		//Note: the reason we have m_clientIPs and m_clientConnections in seperate
		//data structures is because when sf::TcpListener accepts a socket, we only
		//have access to the socket, and can't send or access any other data alongside
		//it (in our case, we need the player's name and port the client is using).
		//When a connection is accepted, it is pushed back to m_clientConnections.
		//
		//When a client sends a JUST_JOINED packet, we can then send data with it - the
		//player's name and port. It is then that the IPInfo containing the player's
		//name and client's port is pushed back to m_clientIPs.

		Server& m_server;
		sf::TcpListener m_listener;
		sf::UdpSocket m_udpSocket;
		std::vector<std::unique_ptr<sf::TcpSocket>> m_clientConnections;
		std::map<const sf::TcpSocket*, IPInfo> m_clientIPs;
		std::vector<std::pair<std::string, std::string>> m_messages;
};


#endif // NETWORKMANAGERSERVER_H_INCLUDED
