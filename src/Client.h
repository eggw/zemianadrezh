#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "NetworkManagerClient.h"

#include "GameInstance.h"
#include "ChatBox.h"

/*
The client class takes an optional pointer to a server.
This pointer should only be passed if the server is local;
if it's passed, the client will use the same resources as the
server.
*/

class Server;

class Client : public GameInstance {
	public:
		Client(sf::RenderWindow& _window, Server* _localServer = nullptr);

		void getInput(sf::Event& _event);
		void update();
		void updateOtherPlayers(Player::EncodedPlayerData _data);

		void sendPackets();
		void receivePackets();

		void respawnPlayer();

		const Player* getPlayer() const;
		const ChatBox* getChatBox() const;

		bool isLocal() const;
	public:
		NetworkManagerClient m_networkManager;
	private:
		Server* m_localServer;
		Player m_player;
		ChatBox m_chatBox;
};

#endif // CLIENT_H_INCLUDED
