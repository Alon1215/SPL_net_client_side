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
    ConnectionHandler& handler;
    ClientDB& myDB;
    int getOpcode(std::string st);
    std::string unify_book_name(std::vector<std::string> &vec);
//    std::string unify_book_name_borrow(std::vector<std::string> &vec);
    std::string fix_body(std::string &body);


public:
    Protocol(ClientDB &db, ConnectionHandler &handler);
    void process_server(std::string &msg);
    void process_keyboard(std::string &msg);
    static std::vector<std::string> input_to_vector(const std::string& str, char delimiter);
    static std::vector<std::string> input_to_vector(const std::string& str);
    void send(std::string topic,std::string body);
    void send_stomp_frame (std::string header, std::string body);

};

//
#endif //SPL_NET_CLIENT_PROTOCOL_H
