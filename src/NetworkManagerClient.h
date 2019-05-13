#ifndef NETWORKMANAGERCLIENT_H_INCLUDED
#define NETWORKMANAGERCLIENT_H_INCLUDED

#include "SFML/Network.hpp"

#include "PacketTypes.h"
#include "PacketSender.h"
#include "Message.h"

class Client;

class NetworkManagerClient {
  public:
	NetworkManagerClient(Client& _client);

	void update();

	void sendPacket(Packet::TCPPacket _type);
	void sendPacket(Packet::UDPPacket _type);
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
	//---------------------------------------------
};

#endif // NETWORKMANAGERCLIENT_H_INCLUDED
