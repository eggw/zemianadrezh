#ifndef PACKETTYPES_H_INCLUDED
#define PACKETTYPES_H_INCLUDED

#include <map>

namespace Packet {

	constexpr unsigned short Port_TCP_Server{7777};
	constexpr unsigned short Port_UDP_Server{7777};

	enum class TCP
	{
		JUST_JOINED,
		QUIT,
		CONNECTIONLOST,
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

	enum class UDP
	{
		//Used by the client to send its
		//position to the server
		DATA_PLAYER,
		DATA_WORLDTIME
	};

	constexpr auto toInt(Packet::TCP _t) {
		return static_cast<std::underlying_type_t<Packet::TCP>>(_t);
	}
	constexpr Packet::TCP toTCPType(int _i) {
		return static_cast<Packet::TCP>(_i);
	}

	constexpr auto toInt(Packet::UDP _t) {
		return static_cast<std::underlying_type_t<Packet::UDP>>(_t);
	}
	constexpr Packet::UDP toUDPType(int _i) {
		return static_cast<Packet::UDP>(_i);
	}
} // namespace Packet

#endif // PACKETTYPES_H_INCLUDED
