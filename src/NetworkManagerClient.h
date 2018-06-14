#ifndef NETWORKMANAGERCLIENT_H_INCLUDED
#define NETWORKMANAGERCLIENT_H_INCLUDED

#include "SFML/Network.hpp"

#include "PacketTypes.h"

class Client;

class NetworkManagerClient {
  public:
  	NetworkManagerClient(Client& _client);

  	void sendPacket(Packet::Type _type);
  	void receivePacket();
  	void connect(sf::IpAddress _ip, int _port);  private:
  	Client& m_client;
	sf::TcpSocket m_serverConnection;
};

#endif // NETWORKMANAGERCLIENT_H_INCLUDED
