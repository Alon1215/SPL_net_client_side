//
// Created by mosesofe@wincs.cs.bgu.ac.il on 09/01/2020.
//

#include "../include/ServerListenerTask.h"
#include "../include/ClientDB.h"

using boost::asio::ip::tcp;

ServerListenerTask::ServerListenerTask(ConnectionHandler &handler, std::string name,ClientDB &db,Protocol &protocol):handler(handler),name(name),db(db),protocol(protocol) {}
void ServerListenerTask::operator()() {
    printf("inside Server-Listener:\n\n"); //TODO: for us, should be deleted

    //while loop, processing until should be terminated:

    while(!db.getIsShouldTerminate1()){ //
        printf("Server-Listener  task operating\n");
        std::string inMsg;

        //in case connectivity went wrong:
        if(!handler.getLine(inMsg)){
            db.setIsShouldTerminate(true);
            break;
        }

        protocol.process_server(inMsg);
    }
}

