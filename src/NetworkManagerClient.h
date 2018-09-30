#ifndef NETWORKMANAGERCLIENT_H_INCLUDED
#define NETWORKMANAGERCLIENT_H_INCLUDED

#include "SFML/Network.hpp"

#include "PacketTypes.h"
#include "PacketSender.h"

class Client;

class NetworkManagerClient {
	public:
		NetworkManagerClient(Client& _client);

		void sendPacket(Packet::Type _type);
		void receivePacket();
		void connect(sf::IpAddress _ip, int _port);

		bool connectionActive() const;

		//This function returns a bool that specifies whether new message
		//was received over the network or not. An optional pointer can be
		//passed that will be assigned the new message.
		bool receivedMessage(std::pair<std::string, std::string>* _ptr = nullptr);
		void clearLastReceivedMessage();

		//See explanation below for details
		//Note: chunkDataReceived can optionally take a pointer to a
		//vector of ints. If it does, the IDs of the chunks that were
		//received will be written to the vector, and so, not all chunks
		//need to be updated whenever one changes.
		bool chunkDataReceived(std::vector<int>* _ids = nullptr) const;
		void setChunkDataProcessed(bool _val);
	private:
		Client& m_client;
		sf::TcpSocket m_serverConnection;

		//m_chunkDataReceived will be set to true (in NetworkManagerClient)
		//whenever we receive any new chunk data. The reason for this is
		//we only need to visually refresh (call RendererChunk.update())
		//when chunks actually change; after we do that, we'll set
		//it back to false.

		//Additionally, the IDs of the received chunks will be written to
		//m_lastReceivedChunks
		bool m_chunkDataReceived{false};
		std::vector<int> m_lastReceivedChunks;
		//Since the world data is sent by packets as a string, this function
		//will do the work of converting it into a vector of ints and setting
		//m_lastReceivedChunks
		void stringIDsToVector(std::string _ids);


		std::pair<std::string, std::string> m_lastReceivedMessage;
};

#endif // NETWORKMANAGERCLIENT_H_INCLUDED
