//
// Created by mosesofe@wincs.cs.bgu.ac.il on 09/01/2020.
//

#include <boost/asio/ip/tcp.hpp>
#include "../include/Protocol.h"
#include "../include/connectionHandler.h"
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>

enum string_code{
    connected,receipt,message,error,
    disconnect,subscribe,unsubscribe, //receipt cases
    returning, bookstatus,taking,someone_has,someone_wish,someone_added   //message cases
};
Protocol::Protocol(ClientDB &db, ConnectionHandler &handler): myDB(db) , handler(handler) {

}



//void Protocol::process_keyboard(std::string  &msg) {
//
//}

void Protocol::process_server(std::string &msg) {
    std::vector<std::string> result; //vector of all lines in input msg
    std::vector<std::string> parse_vec;
    std::vector<std::string> mission_info;
    int opcode2;
    int receipt_num;
    boost::split(result, msg, boost::is_any_of("\n"));
    int opcode = getOpcode(result.at(0));
    switch(opcode) {
        case connected:
            myDB.setIsActive(true);
            std::cout << "Successfully connected to Server!..\n" << std::endl;
            break;
        case message:
            boost::split(parse_vec, result.at(5), boost::is_any_of(" ")); //split message body into words
            opcode2=getOpcode(parse_vec.at(0)); //get first word code
            switch(opcode2){
                case taking:
                    if(parse_vec.at(3) == myDB.getMyName()){ //need to give a book


                    }

                    break;

                case bookstatus:


                    break;
                case returning:
                    break;
                default:

            }



            break;
        case receipt:
            boost::split(parse_vec, result.at(1), boost::is_any_of(":"));
            receipt_num = stoi(parse_vec.at(1)); //get receipt number
            mission_info = myDB.getReceiptMap().at(receipt_num);
            opcode2 = getOpcode(mission_info.at(0)); //get the type of my receipt message
            switch(opcode2){
                case disconnect:
                    myDB.setIsActive(false); //TODO:ofer: check if valid change (here is where we close socket!)
                    handler.close(); //close the socket
                    std::cout << "Successful logout from Server!..\n" << std::endl;
                    break;
                case subscribe:
                    std::cout << "Joined club "
                              << mission_info.at(1) << std::endl;
                    break;
                case unsubscribe:
                    std::cout << "Exited club "
                              << mission_info.at(1) << std::endl;
                    break;
            }
        case error:
            std::cout << "Error occured \n: "
                      << msg << std::endl;
            myDB.setIsActive(false);
            handler.close();

        default: //TODO: should choose what to do in case invalid msg header recieved
            break;
    }

}





std::vector<std::string> Protocol::input_to_vector(const std::string &str, char delimiter) {
    std::string word = "";
    std::vector<std::string> output;
    for (auto x : str)
    {
        if (x == delimiter)
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
    if(st=="ERROR")
        return error;

    return -1; //invalid msg header

}

void Protocol::send(std::string topic, std::string body) {


}



void Protocol::process_keyboard(std::string &msg) {


}









