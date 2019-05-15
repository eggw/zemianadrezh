#ifndef NETWORKMANAGERSERVER_H_INCLUDED
#define NETWORKMANAGERSERVER_H_INCLUDED

#include <memory>
#include <map>

#include "SFML/Network.hpp"

#include "FunctionBinder.h"
#include "PacketTypes.h"
#include "PacketSender.h"
#include "Message.h"

class Server;

class NetworkManagerServer {
  public:
	NetworkManagerServer(Server& _server);

	//There's an explanation that applies to these
	//functions below
	void sendPacket(Packet::TCP _type,
	                sf::TcpSocket*    _recipient = nullptr,
	                bool              _exclude   = false);

	void sendPacket(Packet::UDP  _type,
	                const std::string& _recipientName,
	                bool               _exclude = false);

	void sendChunkData(int _chunkID);

	void receiveTCPPackets();
	void receiveUDPPackets();

	//Sends a message to all connected players
	void sendMessage(const Message& _msg);
	//Handles everything to do with removing a player, including
	//removing it from m_clientIPs and m_clientConnections,
	//calling Server::removePlayer(), and sending a QUIT packet
	//to all connected clients containing the name of the player
	//who was removed
	void removePlayer(sf::TcpSocket* _connection = nullptr);

	void listen();
	void accept();
	void update();

	size_t connectedPlayers() const;

  private:
	//Since the clients that connect to us via UDP will have varying
	//ports, we'll use IPInfo to conveniently store it alongside their IP
	struct IPInfo {
		IPInfo(std::string _n, sf::TcpSocket& _t, unsigned short _p)
		            : playerName{_n},
		              ipAddress{_t.getRemoteAddress()},
		              port{_p} {};

		std::string    playerName;
		sf::IpAddress  ipAddress;
		unsigned short port;
	};

	//Functions -----------------------------------
	//TCP-related
	void sendQuit(std::vector<PacketSharedPtr>& _p);
	void sendDataWorld(std::vector<PacketSharedPtr>& _p);
	void sendChatMessage(std::vector<PacketSharedPtr>& _p);

	void receiveJustJoined(sf::Packet* _p, sf::TcpSocket* _conn);
	void receiveQuit(sf::Packet* _p, sf::TcpSocket* _conn);
	void receiveRequestWorld(sf::Packet* _, sf::TcpSocket* _conn);
	void receiveChatMessage(sf::Packet* _p, sf::TcpSocket* _conn);

	//

	//UDP-related
	void sendDataPlayer(std::vector<PacketSharedPtr>& _p);

	void receiveDataPlayer(sf::Packet* _p, sf::TcpSocket* _conn);
	//

	//Generally, our recipients are either going to be:
	//one specific recipient,
	//all recipients,
	//or all but one recipient.
	//
	//These functions take in a target recipient, defaulting to nullptr
	//(in getTcpRecipients) or an empty string (in getUDPRecipients),
	//and a boolean, defaulting to false.
	//
	//N.B.: The reason getUDPRecipients() takes a string and not a
	//UdpSocket is because it is possible for two players to have
	//the same IP address (such as when testing two local clients).
	//
	//In such a case, we still need to be able to differentiate
	//them, and this cannot be done via just knowing their IP.
	//However, since two players cannot have the same name, we
	//can safely assume their name uniquely identifies them.
	//
	//If the recipient is null (or an empty string), they will return all
	//possible recipients.
	//If _exclude is false, they will only return the recipient specified.
	//If _exclude is true, they will return all recipients except the one
	//specified.
	//
	//These functions are utilised in the sendPacket() functions.
	//
	std::vector<sf::TcpSocket*> getTCPRecipients(
	  sf::TcpSocket* _recipient = nullptr,
	  bool           _exclude   = false);

	std::vector<IPInfo> getUDPRecipients(const std::string& _recipientName = "",
	                                     bool               _exclude = false);

	//removeConnection() is used to remove the matching TcpSocket from
	//m_clientConnections as well as the associated IPInfo in m_clientIPs
	void removeConnection(sf::TcpSocket* _con);

	//---------------------------------------------

	//Data members --------------------------------
	//Note: the reason we have m_clientConnections and m_clientIPs in seperate
	//data structures is because when sf::TcpListener accepts a socket, we only
	//have access to the socket, and can't send or access any other data alongside
	//it (in our case, we need the player's name and port the client is using).
	//When a connection is accepted, it is pushed back to m_clientConnections.
	//
	//When a client sends a JUST_JOINED packet, we can then send data with it - the
	//player's name and port. It is then that the IPInfo containing the player's
	//name and client's port is pushed back to m_clientIPs.

	Server&                                     m_server;
	sf::TcpListener                             m_listener;
	sf::UdpSocket                               m_udpSocket;
	std::vector<std::unique_ptr<sf::TcpSocket>> m_clientConnections;
	std::map<sf::TcpSocket*, IPInfo>            m_clientIPs;
	std::vector<Message>                        m_messages;
	std::string                                 m_lastRemovedPlayer;

	/*
	Similar to the behaviour in NetworkManagerClient, we're going to have 4
	FunctionBinder objects which we'll use to map packet types to callback
	functions.

	The receivers will take in a pointer to the packet to extract relevant
	information from, and a pointer to the TcpSocket uniquely identifying
	the client.

	The senders will take in a reference to a vector of shared packet ptrs
	instead. This is because some callbacks will require multiple packets
	be sent (such as when sending the world, one chunk at a time). After
	all the packets have been prepared, they will be sent in the
	::sendPacket functions.
	*/

	FunctionBinder<Packet::TCP, void, std::vector<PacketSharedPtr>&> m_TCPSender;
	FunctionBinder<Packet::TCP, void, sf::Packet*, sf::TcpSocket*> m_TCPReceiver;

	FunctionBinder<Packet::UDP, void, std::vector<PacketSharedPtr>&> m_UDPSender;
	FunctionBinder<Packet::UDP, void, sf::Packet*, sf::TcpSocket*> m_UDPReceiver;
	//---------------------------------------------
};

#endif // NETWORKMANAGERSERVER_H_INCLUDED
