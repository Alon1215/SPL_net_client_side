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
    Protocol& protocol; //TODO: impelmet protocol
    ClientDB& db;
    std::string name;
public:
    ServerListenerTask(ConnectionHandler &handler, std::string name,ClientDB& db,Protocol &protocol);

    ServerListenerTask(const ServerListenerTask& other);
    std:: vector <std::string> split(std::string st, std::string delimiter);
    void operator()();
    virtual ~ServerListenerTask();
    ServerListenerTask& operator=(const ServerListenerTask& other);


};

//class KeyboardTask {
//private:
//    Packet* packet;
//    Protocol* protocol;
//    ConnectionHandler* connectionHandler;
//    int* loggedin;
//public:
//    KeyboardTask(ConnectionHandler *conHand,Protocol* aProtocol,int* loggedin); TODO::delete
//    KeyboardTask(const KeyboardTask& other);
//    std:: vector<std::string> split(std::string s,std:: string delimiter);
//    void operator()();
//    virtual ~KeyboardTask();
//    KeyboardTask& operator=(const KeyboardTask& other);
//};



#endif //SPL_NET_CLIENT_SERVERLISTENERTASK_H
