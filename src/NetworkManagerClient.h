#ifndef NETWORKMANAGERCLIENT_H_INCLUDED
#define NETWORKMANAGERCLIENT_H_INCLUDED

#include "SFML/Network.hpp"

#include "PacketTypes.h"

class NetworkManagerClient {
  public:
  	NetworkManagerClient();

  	void sendPacket(Packet::Type _type);
  	void receivePacket();



  	void connect(sf::IpAddress _ip, int _port);  private:
	sf::TcpSocket m_serverConnection;
};

#endif // NETWORKMANAGERCLIENT_H_INCLUDED
