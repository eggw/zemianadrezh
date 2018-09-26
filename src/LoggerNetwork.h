#ifndef LOGGERNETWORK_H_INCLUDED
#define LOGGERNETWORK_H_INCLUDED

#include "Singleton.h"

class LoggerNetwork: public Singleton<LoggerNetwork> {

	public:
		LoggerNetwork();

		enum class LOG_SENDER {
			SERVER,
			CLIENT
		};

		enum class LOG_MESSAGE {
			CONNECTION_WAITING,
			CONNECTION_SUCCESS,
			CONNECTION_FAILURE,
			LISTEN_PORT_SUCCESS,
			LISTEN_PORT_FAILURE,
			CONNECTION_LOCALHOST
		};

		enum class LOG_PACKET_DATATRANSFER {
			PACKET_SENT,
			PACKET_RECEIVED
		};

		void log(LOG_SENDER _sender, LOG_MESSAGE _message);
		void logConsole(LOG_SENDER _sender, LOG_MESSAGE _message);
		void logConsole(LOG_SENDER _sender, LOG_PACKET_DATATRANSFER _d, int _packetCode);

};

#endif // LOGGERNETWORK_H_INCLUDED
