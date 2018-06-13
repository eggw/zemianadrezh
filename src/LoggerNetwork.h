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
        CONNECTION_SUCCESS,
        CONNECTION_FAILURE,
        LISTEN_PORT_SUCCESS,
        LISTEN_PORT_FAILURE
    };

    void log(LOG_SENDER _sender, LOG_MESSAGE _message);

  private:
    bool m_init{false};

};

#endif // LOGGERNETWORK_H_INCLUDED
