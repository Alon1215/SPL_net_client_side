//
// Created by mosesofe@wincs.cs.bgu.ac.il on 09/01/2020.
//

#ifndef SPL_NET_CLIENT_PROTOCOL_H
#define SPL_NET_CLIENT_PROTOCOL_H


#include <string>
#include <vector>
#include <condition_variable>
#include "ClientDB.h"
#include "connectionHandler.h"

class Protocol {
private:
    ClientDB& myDB;
    ConnectionHandler& handler;
    int getOpcode(std::string st);
    std::string unify_book_name(std::vector<std::string> &vec);
    std::string unify_book_name_borrow(std::vector<std::string> &vec);
    std::string unify_book_name_taking(std::vector<std::string> &vec);
    std::string fix_body(std::string &body);
    std::string fix_book_name(std::string book);
    std::string unify_book_name_returning(std::vector<std::string> &vec);

public:
    Protocol(ClientDB &db, ConnectionHandler &handler);
    void process_server(std::string &msg);
    void process_keyboard(std::string &msg);
    static std::vector<std::string> input_to_vector(const std::string& str);
    void send(std::string topic,std::string body);
    void send_stomp_frame (std::string header, std::string body);
    void handleExit(int subID, const std::string &topicName);

    void remove_from_all_topics();

    void takingCase(std::vector<std::string> &result, std::vector<std::string> &parse_vec, std::string &book,
                    std::string &topic);

    void bookstatusCase(const std::string &msg, std::vector<std::string> &result, std::vector<std::string> &parse_vec,
                        std::vector<std::string> &books, std::string &topic, std::string &body);

    void returningCase(std::vector<std::string> &result, std::vector<std::string> &parse_vec, std::string &book,
                       std::string &topic);

    void
    wishCase(std::vector<std::string> &result, std::vector<std::string> &parse_vec, std::string &book,
             std::string &topic);

    void
    hasCase(std::vector<std::string> &result, std::vector<std::string> &parse_vec, std::string &book,
            std::string &topic,
            std::string &other_name);

    void addBookCase(std::vector<std::string> &vector_for_input);

    void returnCase(std::string &loaner_name, std::vector<std::string> &vector_for_input);
};


//
#endif //SPL_NET_CLIENT_PROTOCOL_H
