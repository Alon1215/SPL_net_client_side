//
// Created by mosesofe@wincs.cs.bgu.ac.il on 09/01/2020.
//

#ifndef SPL_NET_CLIENT_PROTOCOL_H
#define SPL_NET_CLIENT_PROTOCOL_H


#include <string>

class Protocol {

public:
    void process_server(std::string& msg);
    void process_keyboard(std::string& msg);

};


#endif //SPL_NET_CLIENT_PROTOCOL_H
