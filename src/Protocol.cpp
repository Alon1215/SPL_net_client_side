//
// Created by mosesofe@wincs.cs.bgu.ac.il on 09/01/2020.
//

#include <boost/asio/ip/tcp.hpp>
#include "../include/Protocol.h"
#include "../include/connectionHandler.h"
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>

enum string_code{
    connected,receipt,message,
    disconnect,subscribe,unsubscribe, //receipt cases
    returning, bookstatus,taking,someone_has,someone_wish,someone_added
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
        case message:
            std::string message
            }

            break;
        case receipt:
            std::vector<std::string> receipt;
            boost::split(receipt,result.at(1),boost::is_any_of(":"));
            int receiptnum = stoi(receipt.at(1)); //get receipt number
            std::vector<std::string> missioninfo = datab.getReceiptMap().at(receiptnum);
            int opcode2 = getOpcode(missioninfo.at(0)); //get the type of my receipt message
            switch(opcode2){
                case disconnect:
                    datab.setIsActive(false); //TODO:ofer: check if valid change (here is where we close socket!)
                    handler.close(); //close the socket
                    std::cout << "Successful logout from Server!..\n" << std::endl;
                    break;
                case subscribe:
                    std::cout << "Joined club "
                              << missioninfo.at(1)  << std::endl;
                    break;
                case unsubscribe:
                    std::cout << "Exited club "
                              << missioninfo.at(1)  << std::endl;
                    break;
            }
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
    if(st=="DISCONNECT")
        return disconnect;
    if(st=="SUBSCCRIBE")
        return subscribe;
    if(st=="UNSUBSCRIBE")
        return unsubscribe;
    if(st=="BOOK")
        return bookstatus;
    if(st=="RETURNING")
        return returning;
    if(st=="TAKING")
        return taking;

    return -1;

}



