//
// Created by mosesofe@wincs.cs.bgu.ac.il on 09/01/2020.
//

#ifndef SPL_NET_CLIENT_SERVERLISTENERTASK_H
#define SPL_NET_CLIENT_SERVERLISTENERTASK_H


#include "connectionHandler.h"
#include "Protocol.h"
#include "ClientDB.h"

class ServerListenerTask {
private:
    ConnectionHandler& handler;
    Protocol& protocol;
    ClientDB& db;
    std::string name;
public:
    ServerListenerTask(ConnectionHandler &handler, std::string name,ClientDB& db,Protocol &protocol);
    void operator()();


};



#endif //SPL_NET_CLIENT_SERVERLISTENERTASK_H
