#ifndef NETWORKMANAGERSERVER_H_INCLUDED
#define NETWORKMANAGERSERVER_H_INCLUDED

#include <memory>
#include <map>

#include "SFML/Network.hpp"

#include "PtrCompare.h"
#include "FunctionBinder.h"
#include "PacketTypes.h"
#include "PacketSender.h"
#include "Message.h"
#include "Util/HHMM.h"

class Server;

class NetworkManagerServer {
  public:
	NetworkManagerServer(Server& _server);

	//A null _recipient means the packet will be sent to
	//all connected clients, while _exclude specifies if
	//the packet is to be sent to all connected clients
	//EXCEPT _recipient (if true), or only to _recipient
	//(if false)
	//-----------------------------------
	void sendPacket(Packet::TCP    _type,
	                sf::TcpSocket* _recipient = nullptr,
	                bool           _exclude   = false);

	void sendPacket(Packet::UDP    _type,
	                sf::TcpSocket* _recipient = nullptr,
	                bool           _exclude   = false);
	//-----------------------------------

	void receiveTCPPackets();
	void receiveUDPPackets();

	//Sends a message to all connected players
	void sendMessage(const Message& _msg);
	//Sends the world time (if it has been updated)
	//to all connected players
	void sendWorldTime();

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

	//A wrapper used by getRecipients()
	struct ConnectionData {
		ConnectionData(sf::TcpSocket* _t, IPInfo* _i)
		            : socket{_t}, ipInfo{_i} {};

		sf::TcpSocket* socket{nullptr};
		IPInfo*        ipInfo{nullptr};
	};

	using SocketUPtr = std::unique_ptr<sf::TcpSocket>;
	using IPInfoUPtr = std::unique_ptr<IPInfo>;

	using PacketSharedPtrs = std::vector<PacketSharedPtr>;

	//Functions -----------------------------------
	//TCP-related
	//--------------
	void sendQuit(PacketSharedPtrs& _p, sf::TcpSocket* _conn);
	void sendDataWorld(PacketSharedPtrs& _p, sf::TcpSocket* _conn);
	void sendChatMessage(PacketSharedPtrs& _p, sf::TcpSocket* _conn);

	void receiveJustJoined(sf::Packet* _p, sf::TcpSocket* _conn);
	void receiveQuit(sf::Packet* _p, sf::TcpSocket* _conn);
	void receiveRequestWorld(sf::Packet* _, sf::TcpSocket* _conn);
	void receiveChatMessage(sf::Packet* _p, sf::TcpSocket* _conn);
	//--------------

	//UDP-related
	//--------------
	void sendDataPlayer(PacketSharedPtrs& _p, sf::TcpSocket* _conn);
	void sendDataWorldTime(PacketSharedPtrs& _p, sf::TcpSocket* _conn);

	void receiveDataPlayer(sf::Packet* _p, sf::TcpSocket* _conn);
	//--------------

	//Handles everything to do with removing a player, including
	//calling Server::removePlayer() and sending a QUIT packet
	//containing the name of the player that was removed to all
	//connected clients.
	//
	//Actual removal from m_clientIPs happens when removeClients()
	//is called.
	void markClientForRemoval(sf::TcpSocket* _conn = nullptr);
	//Removes from m_clientIPs any clients that may have been
	//marked for removal, such as when receiving a QUIT packet
	void removeClients();

	std::vector<ConnectionData> getRecipients(
	  sf::TcpSocket* _recipient = nullptr,
	  bool           _exclude   = false);

	IPInfo* getIPInfo(sf::TcpSocket* _conn) const;
	//---------------------------------------------

	//Data members --------------------------------
	//When a connection is accepted, it is pushed back to m_clientIPs. However,
	//its mapped value (IPInfo*) is null and will remain so until a JUST_JOINED
	//packet is received from that client.
	//
	//When a client sends a JUST_JOINED packet, we can then send data with it - the
	//player's name and port. It is then that the IPInfo containing the player's
	//name and client's port is instantiated in m_clientIPs.

	std::map<SocketUPtr, IPInfoUPtr, PtrCompare<sf::TcpSocket>> m_clientIPs;

	Server&              m_server;
	sf::TcpListener      m_listener;
	sf::UdpSocket        m_udpSocket;
	std::vector<Message> m_messages;
	std::string          m_lastRemovedPlayer;
	HHMM                 m_lastTime;

	std::vector<sf::TcpSocket*> m_connectionsToRemove;

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
	::sendPacket functions. They will also take in a TcpSocket ptr speci-
	fying the intended recipient, which needs to be known in certain cases
	(such as to avoid sending a client's own player data back to them).
	*/

	FunctionBinder<Packet::TCP, void, PacketSharedPtrs&, sf::TcpSocket*>
	  m_TCPSender;
	FunctionBinder<Packet::TCP, void, sf::Packet*, sf::TcpSocket*>
	  m_TCPReceiver;

	FunctionBinder<Packet::UDP, void, PacketSharedPtrs&, sf::TcpSocket*>
	  m_UDPSender;
	FunctionBinder<Packet::UDP, void, sf::Packet*, sf::TcpSocket*>
	  m_UDPReceiver;
	//---------------------------------------------
};

#endif // NETWORKMANAGERSERVER_H_INCLUDED
