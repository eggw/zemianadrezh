//////////////////////////////////////////////|
//Contains tests at the end of the file       |
//////////////////////////////////////////////|
#include "debug.h"
//--------------------------------------------|
// (*) Client - Server connection
//--------------------------------------------|
#include "Constants.h"

#include "Client.h"
#include "Server.h"

#include "LoggerNetwork.h"
#include "TextureManager.h"

#include "Tags/PlayerTag.h"
#include "Components/ComponentAnimation.h"
#include "Components/ComponentsPlayer.h"

#include "Keybinds.h"

static constexpr float MIN_ZOOM{0.5f};
static constexpr float MAX_ZOOM{1.0f};
static constexpr float ZOOM_STEP{0.1f};

static constexpr int LEFTMOST{0};
static constexpr int RIGHTMOST{Dimensions::Chunk::X * Dimensions::Block::X *
                               Dimensions::World::X};

static constexpr int VIEW_LEFTMOST{
  (Dimensions::Chunk::X * Dimensions::Block::X) - (7 * Dimensions::Block::X)};

static constexpr int VIEW_RIGHTMOST{
  Dimensions::Chunk::X * Dimensions::Block::X * (Dimensions::World::X - 1) +
  (7 * Dimensions::Block::X)};

Client::Client(sf::IpAddress _serverIP, Server* _localServer)
            : GameInstance(_localServer),
              m_networkManager(*this),
              m_serverIP(_serverIP),
              m_localServer(_localServer),
              m_player{},
              //TODO:
              //ChatBox's second parameter should take in the player's name more...
              //elegantly
              m_interface(m_networkManager,
                          _localServer == nullptr ? "RemotePlayer" :
                                                    "LocalPlayer"),
              m_systemAnimation{},
              m_systemDrawing{},
              m_systemPhysics{LEFTMOST, RIGHTMOST},
              m_systemPlayerMovement{},
              m_view{},
              m_skyView{},
              m_zoomLevel{1.0f} {
	initialisePlayer();

	m_networkManager.connect(m_serverIP, Packet::Port_TCP_Server);
}

Client::~Client() {
	m_networkManager.sendPacket(Packet::TCP::QUIT);
}

void Client::getInput(sf::Event& _event) {
	handleZoom(_event);
	m_interface.getInput(_event);
	m_systemPlayerMovement.getInput(*m_registry, m_player);
}

void Client::update(int _timeslice) {
	updateSystems(_timeslice);
	m_world->update(_timeslice);
	m_networkManager.update();
	m_interface.update(_timeslice);
}

void Client::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	updateViews(target);

	target.setView(m_skyView);
	m_world->drawBackground(target, states);

	target.setView(m_view);
	m_world->drawChunks(target, states);

	target.draw(m_interface, states);
	m_systemDrawing.draw(*m_registry, target, states);
}

void Client::updateSystems(int _timeslice) {
	m_systemAnimation.update(_timeslice, *m_registry);
	m_systemPhysics.update(_timeslice, *m_registry);
}

void Client::sendPlayerPacket() {
	//Ensure that a reasonable rate of packet sending is maintained.
	static sf::Clock c;
	static float     secondsBetweenPackets{0.015};

	if (c.getElapsedTime().asSeconds() >= secondsBetweenPackets) {
		m_networkManager.sendPacket(Packet::UDP::DATA_PLAYER);
		c.restart();
	}
}

void Client::receivePackets() {
	m_networkManager.receiveUDPPackets();
	m_networkManager.receiveTCPPackets();
}

bool Client::isConnected() const {
	return m_networkManager.connectionActive();
}

bool Client::isLocal() const {
	return m_localServer != nullptr;
}

void Client::handleZoom(sf::Event& _event) {
	if (_event.type == sf::Event::KeyPressed) {
		if (_event.key.code == Key::ZOOM_IN && m_zoomLevel > MIN_ZOOM) {
			m_zoomLevel -= ZOOM_STEP;
		}
		else if (_event.key.code == Key::ZOOM_OUT && m_zoomLevel < MAX_ZOOM) {
			m_zoomLevel += ZOOM_STEP;
		}
	}
}

void Client::initialisePlayer() {
	m_player = m_registry->create();
	ComponentsPlayer p{};
	p.compName.m_name =
	  (m_localServer == nullptr ? "RemotePlayer" : "LocalPlayer");
	addPlayer(p, m_player);
}

void Client::respawnPlayer() {
	auto& playerPos = m_registry->get<ComponentPosition>(m_player).m_position;
	playerPos.x     = m_world->getCenter().x;
	playerPos.y     = 0;
}

void Client::updateViews(sf::RenderTarget& _target) const {
	m_view.reset(
	  {0, 0, float(_target.getSize().x), float(_target.getSize().y)});

	m_skyView.reset(
	  {0, 0, float(_target.getSize().x), float(_target.getSize().y)});

	auto viewCenter{getPlayerPosition()};
	float VIEW_RIGHTMOST_CURRENT {VIEW_RIGHTMOST + ((1 - m_zoomLevel) * (RIGHTMOST - VIEW_RIGHTMOST))};

	if (viewCenter.x < VIEW_LEFTMOST * m_zoomLevel) {
		viewCenter.x = VIEW_LEFTMOST * m_zoomLevel;
	}
	else if (viewCenter.x > VIEW_RIGHTMOST_CURRENT) {
		viewCenter.x = VIEW_RIGHTMOST_CURRENT;
	}

	m_view.setCenter(viewCenter);
	m_view.zoom(m_zoomLevel);

	m_skyView.setCenter(
	  {float(_target.getSize().x / 2), float(_target.getSize().y / 2)});
}

sf::Vector2f Client::getPlayerPosition() const {
	return m_registry->get<ComponentPosition>(m_player).m_position;
}

const std::string Client::getPlayerName() const {
	return m_registry->get<ComponentName>(m_player).m_name;
}

/////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------TESTS-------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////
TEST_CASE("Testing client-server connection") {
	sf::IpAddress ip{sf::IpAddress::LocalHost};
	Server        server{true};

	SUBCASE("testing server size before joining") {
		CHECK(server.connectedPlayers() == 0);
	}

	SUBCASE("creating client and connecting to server") {
		Client client{ip, &server};

		//Because we're testing a local server-client connection, we're
		//going to want to be sure that the server accepts the connection.
		//
		//If we don't use a while loop, it sometimes won't work due to the
		//procedural nature of calling these functions just once. In a real
		//scenario, the server will be constantly accepting connections.

		while (server.connectedPlayers() == 0) {
			server.acceptConnections();
		}

		CHECK(client.isConnected());
		CHECK(client.isLocal());

		SUBCASE("testing server size after joining") {
			CHECK(server.connectedPlayers() == 1);
		}
	}

	//Client is out of scope, the destructor should have been called
	//and thus a QUIT packet sent to the server
	server.receivePackets();

	SUBCASE("testing server size after client quit") {
		CHECK(server.connectedPlayers() == 0);
	}
}
/////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////
