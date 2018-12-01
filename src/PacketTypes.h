#ifndef PACKETTYPES_H_INCLUDED
#define PACKETTYPES_H_INCLUDED

#include <map>

namespace Packet {

	constexpr unsigned short Port_TCP_Server	{ 7777 };
	constexpr unsigned short Port_UDP_Server	{ 7777 };


	enum class TCPPacket
	{
		JUSTJOINED,
		//Used when sending messages
		CHAT_MESSAGE,
		//Used by the client to send
		//a request for the world data
		REQUEST_WORLD,
		//Used by the server to send the
		//world data
		DATA_WORLD,
		//Used by the server to reset
		//a player's position
		RESPAWN_PLAYER
	};

	enum class UDPPacket
	{
		//Used by the client to send its
		//position to the server
		DATA_PLAYER,
	};

	constexpr auto toInt(TCPPacket _t) {
		return static_cast<std::underlying_type_t<TCPPacket>>(_t);
	}
	constexpr TCPPacket toTCPType(int _i) {
		return static_cast<TCPPacket>(_i);
	}

	constexpr auto toInt(UDPPacket _t) {
		return static_cast<std::underlying_type_t<UDPPacket>>(_t);
	}
	constexpr UDPPacket toUDPType(int _i) {
		return static_cast<UDPPacket>(_i);
	}
}

#endif // PACKETTYPES_H_INCLUDED
