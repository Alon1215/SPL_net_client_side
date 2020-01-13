//
// Created by michalon@wincs.cs.bgu.ac.il on 11/01/2020.
//

#ifndef SPL_NET_CLIENT_KEYBOARDLISTENERTASK_H
#define SPL_NET_CLIENT_KEYBOARDLISTENERTASK_H


#include "connectionHandler.h"
#include "Protocol.h"
#include "ClientDB.h"

class KeyboardListenerTask {
private:
    ConnectionHandler& handler;
    ClientDB& db;
    std::string name;
    bool shouldTerminate;
    Protocol &protocol;

public:
    KeyboardListenerTask(ConnectionHandler &handler, std::string name, ClientDB& db,Protocol &protocol);
//    std:: vector <std::string> split(std::string st, std::string delimiter); //TODO: ALON 13.1 why?
    void operator()();


};


#endif //SPL_NET_CLIENT_KEYBOARDLISTENERTASK_H
