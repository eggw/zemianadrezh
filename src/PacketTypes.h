#ifndef PACKETTYPES_H_INCLUDED
#define PACKETTYPES_H_INCLUDED

#include <map>

namespace Packet {

	constexpr int Port_TCP_Server	{ 7777 };
	constexpr int Port_TCP_Client	{ 7777 };

	constexpr int Port_UDP_Server	{ 7777 };
	constexpr int Port_UDP_Client	{ 7878 };

	enum class Type
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
		//Used by the client to send its
		//position to the server
		DATA_PLAYER,
		//Used by the server to reset
		//a player's position
		RESPAWN_PLAYER
	};

	enum class SocketType
	{
		TCP,
		UDP
	};

	static const std::map<Type, SocketType> PacketTypeMap =
	{
		{Type::JUSTJOINED,      SocketType::TCP},
		{Type::CHAT_MESSAGE,    SocketType::TCP},
		{Type::REQUEST_WORLD,   SocketType::TCP},
		{Type::DATA_WORLD,      SocketType::TCP},
		{Type::DATA_PLAYER,     SocketType::UDP},
		{Type::RESPAWN_PLAYER,  SocketType::TCP}
	};

	constexpr auto toInt(Type _t) {
		return static_cast<std::underlying_type_t<Type>>(_t);
	}

	constexpr Type toType(int _i) {
		return static_cast<Type>(_i);
	}

}

#endif // PACKETTYPES_H_INCLUDED
