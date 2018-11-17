#include "NetworkManagerClient.h"

#include <sstream>

#include "LoggerNetwork.h"

#include "Client.h"

NetworkManagerClient::NetworkManagerClient(Client& _client)
    :m_client(_client) {

    m_serverConnection.setBlocking(false);
}

void NetworkManagerClient::sendPacket(Packet::Type _type) {
    int packetCode = Packet::toInt(_type);
    PacketSharedPtr packet(new sf::Packet());
    *packet << packetCode;

    switch(_type) {

        //////////////////////////////////////////////////////////////////////////////
        case Packet::Type::JUSTJOINED: {
            Player::EncodedPlayerData playerData = m_client.getPlayer()->encodeData();
            *packet << playerData.playerName;

            PacketSender::get_instance().send(&m_serverConnection, packet);

            break;
        }
        //////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////
        case Packet::Type::REQUEST_WORLD: {
            PacketSender::get_instance().send(&m_serverConnection, packet);
            break;
        }
        //////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////
        case Packet::Type::DATA_PLAYER: {
            Player::EncodedPlayerData playerData = m_client.getPlayer()->encodeData();
            *packet << playerData.playerName;
            *packet << playerData.facingLeft;
            *packet << playerData.speed;
            *packet << playerData.positionX;
            *packet << playerData.positionY;

            PacketSender::get_instance().send(&m_serverConnection, packet);

            break;
        }
        //////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////
        case Packet::Type::CHAT_MESSAGE: {
            auto message = m_client.getPendingMessage();

            *packet << message.first;
            *packet << message.second;

            PacketSender::get_instance().send(&m_serverConnection, packet);

            break;
        }
        //////////////////////////////////////////////////////////////////////////////

        default:
            break;
    }


    LoggerNetwork::get_instance().logConsole(LoggerNetwork::LOG_SENDER::CLIENT,
            LoggerNetwork::LOG_PACKET_DATATRANSFER::PACKET_SENT,
            packetCode);
}


void NetworkManagerClient::receivePacket() {
    int packetCode;
    PacketUPtr packet(new sf::Packet());

    if(m_serverConnection.receive(*packet) == sf::Socket::Status::Done) {
        *packet >> packetCode;
        Packet::Type packetType{Packet::toType(packetCode)};
        LoggerNetwork::get_instance().logConsole(LoggerNetwork::LOG_SENDER::CLIENT,
                LoggerNetwork::LOG_PACKET_DATATRANSFER::PACKET_RECEIVED,
                packetCode);

        switch(packetType) {
            //////////////////////////////////////////////////////////////////////////////
            case Packet::Type::DATA_WORLD: {

                World::EncodedWorldData worldData;

                *packet >> worldData.chunkIDs;
                *packet >> worldData.invisibleBlocks;

                m_client.parseWorldData(worldData);

                stringIDsToVector(worldData.chunkIDs);
                setChunkDataProcessed(false);
                break;
            }
            //////////////////////////////////////////////////////////////////////////////

            //////////////////////////////////////////////////////////////////////////////
            case Packet::Type::DATA_PLAYER: {
                Player::EncodedPlayerData playerData;

                *packet >> playerData.playerName;
                *packet >> playerData.facingLeft;
                *packet >> playerData.speed;
                *packet >> playerData.positionX;
                *packet >> playerData.positionY;

                m_client.updateOtherPlayers(playerData);
                break;
            }
            //////////////////////////////////////////////////////////////////////////////

            //////////////////////////////////////////////////////////////////////////////
            case Packet::Type::CHAT_MESSAGE: {
                std::string message;
                std::string sender;
                *packet >> message;
                *packet >> sender;

                m_lastReceivedMessage = std::make_pair(message, sender);
                break;
            }
            //////////////////////////////////////////////////////////////////////////////


            default:
                break;
        }
    }
}

void NetworkManagerClient::connect(sf::IpAddress _ip, int _port) {
    sf::Socket::Status status = m_serverConnection.connect(_ip, _port);

    if(status != sf::Socket::Done) {
        LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::CLIENT,
                                          LoggerNetwork::LOG_MESSAGE::CONNECTION_WAITING);
    }
    else {
        LoggerNetwork::get_instance().log(LoggerNetwork::LOG_SENDER::CLIENT,
                                          LoggerNetwork::LOG_MESSAGE::CONNECTION_SUCCESS);

        sendPacket(Packet::Type::JUSTJOINED);
        sendPacket(Packet::Type::REQUEST_WORLD);
    }
}

bool NetworkManagerClient::connectionActive() const {
    return m_serverConnection.getRemoteAddress() != sf::IpAddress::None;
}

bool NetworkManagerClient::receivedMessage(std::pair<std::string, std::string>* _ptr) {
    bool valid {m_lastReceivedMessage.first != ""};

    if(_ptr != nullptr) {
        *_ptr = m_lastReceivedMessage;
    }

    return valid;
}

void NetworkManagerClient::clearLastReceivedMessage() {
    m_lastReceivedMessage.first = "";
    m_lastReceivedMessage.second = "";
}


bool NetworkManagerClient::chunkDataReceived(std::vector<int>* _ids) const {
    if(_ids != nullptr) {
        *_ids = m_lastReceivedChunks;
    }

    return m_chunkDataReceived;
}

void NetworkManagerClient::setChunkDataProcessed(bool _val) {
    m_chunkDataReceived = !_val;
}

void NetworkManagerClient::update() {
    PacketSender::get_instance().update();
}


void NetworkManagerClient::stringIDsToVector(std::string _ids) {
    m_lastReceivedChunks.clear();

    std::replace(_ids.begin(), _ids.end(), '%', ' ' );

    std::stringstream ss(_ids);
    std::copy(	std::istream_iterator<int>(ss),
                std::istream_iterator<int>(),
                back_inserter(m_lastReceivedChunks));
}
