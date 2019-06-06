#ifndef NETWORKMANAGERCLIENT_H_INCLUDED
#define NETWORKMANAGERCLIENT_H_INCLUDED

#include "FunctionBinder.h"
#include "SFML/Network.hpp"

#include "PacketTypes.h"
#include "PacketSender.h"
#include "Message.h"

class Client;

class NetworkManagerClient {
  public:
	NetworkManagerClient(Client& _client);

	void update();

	void sendPacket(Packet::TCP _type);
	void sendPacket(Packet::UDP _type);

	void receiveTCPPackets();
	void receiveUDPPackets();

	void connect(const sf::IpAddress& _ip, unsigned short _port);
	bool connectionActive() const;

	//This function returns a bool that specifies whether new message
	//was received over the network or not. An optional pointer can be
	//passed that will be assigned the new message.
	bool receivedMessage(Message* _ptr = nullptr);
	void clearLastReceivedMessage();

	//When we send a CHAT_MESSAGE packet, it sends the contents of
	//m_messageToSend. This function can be used to set that variable
	//(such as by the UserInterface class) before a chat message is sent.
	void setMessageToSend(Message _msg);

  private:
	//Functions -----------------------------------

	//TCP-related ----------------
	void sendJustJoined(sf::Packet* _p);
	void sendChatMessage(sf::Packet* _p);

	void receiveQuit(sf::Packet* _p);
	void receiveConnectionLost(sf::Packet* _p);
	void receiveChatMessage(sf::Packet* _p);
	void receiveDataWorld(sf::Packet* _p);
	void receiveRespawnPlayer(sf::Packet* _p);
	//----------------------------

	//UDP-related ----------------
	void sendDataPlayer(sf::Packet* _p);

	void receiveDataPlayer(sf::Packet* _p);
	void receiveDataWorldTime(sf::Packet* _p);
	//----------------------------

	//Data members --------------------------------
	Client& m_client;
	//m_playerSpawned is set to true once a RESPAWN::PLAYER packet
	//has been received at least once.
	//
	//All UDP packet sending is disabled until this is set to true.
	//This is to make sure that the player is spawned in the correct
	//position before sending out any further position updates.
	bool m_playerSpawned;
	//m_connectionActive is set to true once there is an active
	//connection to a server, and set to false if the connection
	//is lost.
	bool          m_connectionActive;
	sf::UdpSocket m_udpSocket;
	sf::TcpSocket m_serverConnection;

	Message m_lastReceivedMessage;
	Message m_messageToSend;

	//These will basically serve as registries to which packet types
	//(TCP and UDP) can be mapped to functions. The functions can be
	//called when a packet is sent or received as appropriate, and
	//can be given an argument (typical use case is sf::Packet*).
	//
	//For each protocol (TCP, UDP), and for both sending and receiving,
	//we have a map with the packet type as its key and a function we'll
	//call when that packet is sent or received.
	//
	//Note that not all packets will have defined behaviours. For example,
	//the client will never send DATA_WORLD to the server, so attempting
	//to do so will not cause anything to happen.
	FunctionBinder<Packet::TCP, void, sf::Packet*> m_TCPSender;
	FunctionBinder<Packet::TCP, void, sf::Packet*> m_TCPReceiver;

	FunctionBinder<Packet::UDP, void, sf::Packet*> m_UDPSender;
	FunctionBinder<Packet::UDP, void, sf::Packet*> m_UDPReceiver;
	//---------------------------------------------
};

#endif // NETWORKMANAGERCLIENT_H_INCLUDED
