#include "LoggerNetwork.h"

#include <fstream>
#include <time.h>

#include <map>
#include <iostream>

static std::map<LoggerNetwork::LOG_SENDER, std::string> senderMap;
static std::map<LoggerNetwork::LOG_MESSAGE, std::string> messageMap;

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
    senderMap.emplace(LOG_SENDER::CLIENT, "Client");
    senderMap.emplace(LOG_SENDER::SERVER, "Server");

    messageMap.emplace(LOG_MESSAGE::CONNECTION_FAILURE, "connection failure!");
    messageMap.emplace(LOG_MESSAGE::CONNECTION_SUCCESS, "connection established!");
    messageMap.emplace(LOG_MESSAGE::LISTEN_PORT_FAILURE, "failed to listen to port 7777");
    messageMap.emplace(LOG_MESSAGE::LISTEN_PORT_SUCCESS, "listening on port 7777...");
    messageMap.emplace(LOG_MESSAGE::CONNECTION_LOCALHOST, "connected to local server!");

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

    output << senderMap[_sender] << ": ";
    output << messageMap[_message];

    output << "\n";

    output.close();

    return;
}

void LoggerNetwork::logConsole(LOG_SENDER _sender, LOG_MESSAGE _message){
    std::cout << senderMap[_sender] << ": " << messageMap[_message] << std::endl;
}
