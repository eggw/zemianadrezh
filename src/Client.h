#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include <entt/entity/registry.hpp>

#include "Interface/UserInterface.h"
#include "NetworkManagerClient.h"
#include "GameInstance.h"

#include "Systems/SystemAnimation.h"
#include "Systems/SystemDrawing.h"
#include "Systems/SystemPhysics.h"
#include "Systems/SystemPlayerMovement.h"

#include "RendererChunk.h"

/*
The client class takes an optional pointer to a server.
This pointer should only be passed if the server is local;
if it's passed, the client will share the same resources
(world, registry) as the server.
*/

class Server;

class Client
            : public GameInstance
            , public sf::Drawable {
	friend class NetworkManagerClient;

  public:
	Client(sf::IpAddress _serverIP, Server* _localServer = nullptr);
	~Client();

	void getInput(sf::Event& _event);
	void update(int _timeslice);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void receivePackets();
	void sendPlayerPacket();

	bool isConnected() const;
	bool isLocal() const;

  private:
	//Functions -----------------------------------
	void updateSystems(int _timeslice);
	void handleZoom(sf::Event& _event);
	void initialisePlayer();
	//For now, this sets the player's position to the center of
	//the world (x-axis), on the surface (y-axis 0)
	void respawnPlayer();
	void updateViews(sf::RenderTarget& _target) const;

	sf::Vector2f      getPlayerPosition() const;
	const std::string getPlayerName() const;
	//---------------------------------------------

	//Data members --------------------------------
	NetworkManagerClient m_networkManager;
	sf::IpAddress        m_serverIP;
	Server*              m_localServer;
	entt::entity         m_player;
	UserInterface        m_interface;

	SystemAnimation      m_systemAnimation;
	SystemDrawing        m_systemDrawing;
	SystemPhysics        m_systemPhysics;
	SystemPlayerMovement m_systemPlayerMovement;

	//m_view is the view used to draw chunks, players, items, etc.
	mutable sf::View m_view;
	//m_skyView is the view used to draw the background, sun/moon, etc.
	mutable sf::View m_skyView;

	float m_zoomLevel;
	//---------------------------------------------
};

#endif // CLIENT_H_INCLUDED
