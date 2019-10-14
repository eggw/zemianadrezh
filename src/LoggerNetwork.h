#ifndef LOGGERNETWORK_H_INCLUDED
#define LOGGERNETWORK_H_INCLUDED

#include <SPSS/Util/Singleton.h>

class LoggerNetwork : public spss::Singleton<LoggerNetwork> {
  public:
	LoggerNetwork();

	enum class LOG_SENDER
	{
		SERVER,
		CLIENT
	};

	enum class LOG_MESSAGE
	{
		CONNECTION_WAITING,
		CONNECTION_SUCCESS,
		CONNECTION_FAILURE,
		CONNECTION_BLOCKED,
		LISTEN_PORT_SUCCESS,
		LISTEN_PORT_FAILURE,
		BIND_PORT_SUCCESS,
		BIND_PORT_FAILURE,
		CONNECTION_LOCALHOST
	};

	void log(LOG_SENDER _sender, LOG_MESSAGE _message);
};

#endif // LOGGERNETWORK_H_INCLUDED
