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
		Client(	sf::RenderWindow& _window,
				sf::IpAddress _serverIP,
				Server* _localServer = nullptr);

		void getInput(sf::Event& _event);
		void update();
		void updateOtherPlayers(Player::EncodedPlayerData _data);

		void sendPackets();
		void receivePackets();

		void requestWorldChunks();

		void respawnPlayer();

		const Player* getPlayer() const;
		const ChatBox* getChatBox() const;

		bool isConnected() const;
		bool isLocal() const;

		std::vector<std::pair<bool,std::pair<std::string, std::string>>>& getPendingMessages();
	public:
		NetworkManagerClient m_networkManager;
	private:
		sf::IpAddress m_serverIP;
		Server* m_localServer;
		Player m_player;
		ChatBox m_chatBox;

		//A vector containing a pair of bool and another pair.
		//The bool is false if the message hasn't been verified to have been sent, by
		//checking sf::Socket::Status, and true if it has been sent. See function
		//handlePendingMessages() for more information.

		//The inner pair is the message itself: first the message, then the sender
		std::vector<std::pair<bool,std::pair<std::string, std::string>>> m_pendingMessages;

		void handlePendingMessages();
};

#endif // CLIENT_H_INCLUDED
