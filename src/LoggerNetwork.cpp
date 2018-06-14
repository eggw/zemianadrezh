#include "LoggerNetwork.h"

#include <fstream>
#include <time.h>

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

LoggerNetwork::LoggerNetwork() {

}


void LoggerNetwork::log(LOG_SENDER _sender, LOG_MESSAGE _message) {
    std::ofstream output;
    output.open("log_network.txt", std::fstream::app);

    if(!m_init) {
        output << "\n";
        output << "--------------------------------------------------";
        output << "\n";
        output << "Log begin at " << currentDateTime() << "\n";
        output << "--------------------------------------------------";
        output << "\n";
        m_init = true;
    }

    output << currentDateTime() << " - ";

    switch(_sender) {
    case LoggerNetwork::LOG_SENDER::CLIENT:
        output << "Client: ";
        break;
    case LoggerNetwork::LOG_SENDER::SERVER:
        output << "Server: ";
        break;
    }

    switch(_message) {
    case LoggerNetwork::LOG_MESSAGE::CONNECTION_FAILURE:
        output << "connection failure!";
        break;
    case LoggerNetwork::LOG_MESSAGE::CONNECTION_SUCCESS:
        output << "connection established!";
        break;

    case LoggerNetwork::LOG_MESSAGE::LISTEN_PORT_FAILURE:
        output << "failed to listen to port 7777";
        break;

    case LoggerNetwork::LOG_MESSAGE::LISTEN_PORT_SUCCESS:
        output << "listening to port 7777...";
        break;

    case LoggerNetwork::LOG_MESSAGE::CONNECTION_LOCALHOST:
        output << "connected to local server!";
        break;
    }

    output << "\n";

    output.close();

    return;
}
