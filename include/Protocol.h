//
// Created by mosesofe@wincs.cs.bgu.ac.il on 09/01/2020.
//

#ifndef SPL_NET_CLIENT_PROTOCOL_H
#define SPL_NET_CLIENT_PROTOCOL_H


#include <string>
#include <vector>
#include "ClientDB.h"
#include "connectionHandler.h"

class Protocol {
private:
    ClientDB& myDB;
    ConnectionHandler& handler;
    static int getOpcode(std::string st);


public:
    Protocol(ClientDB &db, ConnectionHandler &handler);
    void process_server(std::string &msg);
    void process_keyboard(std::string &msg);
    static std::vector<std::string> input_to_vector(const std::string& str, char delimiter);
    static std::vector<std::string> input_to_vector(const std::string& str);



};

//
#endif //SPL_NET_CLIENT_PROTOCOL_H
