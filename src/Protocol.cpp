//
// Created by mosesofe@wincs.cs.bgu.ac.il on 09/01/2020.
//

#include <boost/asio/ip/tcp.hpp>
#include "../include/Protocol.h"
#include "../include/connectionHandler.h"
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>

enum string_code{
    connected,receipt,message
};
Protocol::Protocol(ClientDB &db, ConnectionHandler &handler): datab(db) ,handler(handler) {}



void Protocol::process_keyboard(std::string  &msg) {

}

void Protocol::process_server(std::string &msg) {
    std::vector<std::string> result;
    boost::split(result, msg, boost::is_any_of("\n"));
    int opcode = getOpcode(result.at(0));
    switch(opcode) {
        case connected:
            datab.setIsActive(true);
            std::cout << "Successfully connected to Server!..\n" << std::endl;

            break;
        case receipt:
            std::vector<std::string> receipt;
            boost::split(receipt,result.at(1),boost::is_any_of(":"));
            int receiptnum = stoi(receipt.at(1)); //get receipt number
            std::vector<std::string> missioninfo = datab.getReceiptMap().at(receiptnum);



            break;
        case message:
            break;
        default:
            break;

    }



}


std::vector<std::string> Protocol::input_to_vector(const std::string &str) {
    std::string word = "";
    std::vector<std::string> output;
    for (auto x : str)
    {
        if (x == ' ')
        {
            std::string newWord = word;
            output.push_back(word);
            word = "";
        }
        else
        {
            word = word + x;
        }
    }
    output.push_back(word);
    return output;
}

int Protocol::getOpcode(std::string st) {
    if(st=="CONNECTED")
        return connected;
    if(st=="RECEIPT")
        return receipt;
    if(st=="MESSAGE")
        return message;
    return 0;

}



