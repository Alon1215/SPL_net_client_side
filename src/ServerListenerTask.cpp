//
// Created by mosesofe@wincs.cs.bgu.ac.il on 09/01/2020.
//

#include "../include/ServerListenerTask.h"
#include "../include/ClientDB.h"

using boost::asio::ip::tcp;

ServerListenerTask::ServerListenerTask(ConnectionHandler &handler, std::string name,ClientDB &db,Protocol &protocol):handler(handler),name(name),db(db),protocol(protocol) {}
void ServerListenerTask::operator()() {
    while(db.getIsShouldTerminate1()){ //
        std::string inMsg;
        if(!handler.getLine(inMsg)){
            db.setIsActive(false);
            break;
        }
        protocol.process_server(inMsg);
    }
}

//