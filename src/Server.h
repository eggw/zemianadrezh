#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <memory>

#include "NetworkManagerServer.h"

#include "GameInstance.h"

class Server : public GameInstance {
	friend class NetworkManagerServer;

  public:
	//todo: pass world settings to constructor
	Server(bool _allowConnections);
	~Server();

	//Note that Server doesn't have a sendPackets() function.
	//This is because the server doesn't send any unsolicited
	//packets; rather, it responds to packets sent by Clients
	void receivePackets();
	void update(int _timeslice);
	bool connectionsAllowed() const;

  private:
	//Data members --------------------------------
	NetworkManagerServer m_networkManager;
	bool                 m_allowConnections;
	//---------------------------------------------
};

#endif // SERVER_H_INCLUDED
