#include "LoggerNetwork.h"

#include <fstream>
#include <time.h>
#include <map>
#include <string>

#include "PacketTypes.h"

static std::map<LoggerNetwork::LOG_SENDER, std::string>  senderMap;
static std::map<LoggerNetwork::LOG_MESSAGE, std::string> messageMap;

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
	time_t    now = time(0);
	struct tm tstruct;
	char      buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}

LoggerNetwork::LoggerNetwork() {
	senderMap.emplace(LOG_SENDER::CLIENT, "Client");
	senderMap.emplace(LOG_SENDER::SERVER, "Server");

	messageMap.emplace(LOG_MESSAGE::CONNECTION_WAITING, "awaiting response...");
	messageMap.emplace(LOG_MESSAGE::CONNECTION_FAILURE, "connection failure!");
	messageMap.emplace(LOG_MESSAGE::CONNECTION_SUCCESS,
	                   "connection established!");
	messageMap.emplace(LOG_MESSAGE::CONNECTION_BLOCKED,
	                   "unauthorised connection blocked!");
	messageMap.emplace(LOG_MESSAGE::LISTEN_PORT_FAILURE,
	                   "failed to listen to port ");
	messageMap.emplace(LOG_MESSAGE::LISTEN_PORT_SUCCESS, "listening on port ");
	messageMap.emplace(LOG_MESSAGE::BIND_PORT_FAILURE,
	                   "failed to bind to port ");
	messageMap.emplace(LOG_MESSAGE::BIND_PORT_SUCCESS, "bound to port ");
	messageMap.emplace(LOG_MESSAGE::CONNECTION_LOCALHOST,
	                   "connected to local server!");
}

void LoggerNetwork::log(LOG_SENDER _sender, LOG_MESSAGE _message) {
	static bool init{false};

	std::ofstream output;
	output.open("log_network.txt", std::fstream::app);

	if (!init) {
		output << "\n";
		output << "--------------------------------------------------";
		output << "\n";
		output << "Log begin at " << currentDateTime() << "\n";
		output << "--------------------------------------------------";
		output << "\n";
		init = true;
	}

	output << currentDateTime() << " - ";

	output << senderMap[_sender] << ": ";
	output << messageMap[_message];

	if (_message == LOG_MESSAGE::LISTEN_PORT_FAILURE ||
	    _message == LOG_MESSAGE::LISTEN_PORT_SUCCESS) {
		output << Packet::Port_TCP_Server;
	}
	/*
	else if (_message == LOG_MESSAGE::BIND_PORT_FAILURE || _message == LOG_MESSAGE::BIND_PORT_SUCCESS) {
	    if (_sender == LOG_SENDER::CLIENT) {
	        output << Packet::Port_UDP_Client;
	    }
	    else {
	        output << Packet::Port_UDP_Server;
	    }
	}*/

	output << "\n";

	output.close();

	return;
}
