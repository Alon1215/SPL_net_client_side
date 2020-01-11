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
    returning, bookstatus,taking,has,wish,  //message cases
    LOGIN, JOIN, EXIT, ADD_BOOK, BORROW, RETURN, GENRE, LOGOUT //for keyboard

};
Protocol::Protocol(ClientDB &db, ConnectionHandler &handler): myDB(db) , handler(handler) {

}



//void Protocol::process_keyboard(std::string  &msg) {

//}

void Protocol::process_server(std::string &msg) {
    std::vector<std::string> result; //vector of all lines in input msg
    std::vector<std::string> parse_vec;
    std::vector<std::string> mission_info;
    std::vector<std::string> books;
    std::string book;
    std::string topic;
    std::string body;
    int receipt_num;
    std::string other_name;
    boost::split(result, msg, boost::is_any_of("\n"));
    int opcode = getOpcode(result.at(0));
    int opcode2;
    int opcode3;
    switch(opcode) {
        case connected:
            myDB.setIsActive(true);
            std::cout << "Successfully connected to Server!..\n" << std::endl;
            break;
        case message:
            boost::split(parse_vec, result.at(5), boost::is_any_of(" ")); //split message body into words
            opcode2=getOpcode(parse_vec.at(0)); //get first word code
            switch(opcode2) {
                case taking:
                    if (parse_vec.at(3) == myDB.getMyName()) { //need to give a book
                        book = parse_vec.at(1);
                        boost::split(parse_vec, result.at(3), boost::is_any_of(":")); //get topic
                        topic = parse_vec.at(1);
                        if(myDB.remove_book_from_Inv(book,topic)){
                            std::cout << "Borrowed " <<book <<  std::endl;
                        }
                    }

                    break;

                case bookstatus:
                    body = myDB.getMyName() + ":";
                    boost::split(parse_vec, result.at(3), boost::is_any_of(":")); //get topic
                    topic = parse_vec.at(1);
                    books = myDB.getMyInventory().at(topic);
                    for (std::string book: books)
                        body = body + book + ",";

                    send(topic, body);//send frame
                    break;
                case returning:
                    if(parse_vec.size()>=4 && parse_vec.at(3) ==myDB.getMyName()){ //if book is being returned to me
                        book = parse_vec.at(1);
                        boost::split(parse_vec, result.at(3), boost::is_any_of(":")); //get topic //TODO:check that this wont change book variable value
                        topic = parse_vec.at(1);
                        myDB.add_book_to_Inv(book,topic); //take book back to inv
                    }
                    break;
                default:
                    opcode3 = getOpcode(parse_vec.at(1));
                    book = parse_vec.at(4); //TODO:maybe move back to avoid double code
                    switch (opcode3){
                        case wish:


                            if(myDB.inv_contains_book(book,topic)){
                                send(topic,myDB.getMyName()+" has "+book); //if i have this book send has frame
                            }
                            break;
                        case has:
                            other_name = parse_vec.at(0);
                            if(myDB.getMyName()!=other_name){ //act only if this isn't my message
                                boost::split(parse_vec, result.at(3), boost::is_any_of(":")); //get topic //TODO:check that this wont change book variable value
                                topic = parse_vec.at(1);
                                if(myDB.wishList_contains(book)){
                                    myDB.remove_book_from_wishList(book,topic);
                                    myDB.add_book_to_Inv(book,topic); //add the loaned book
                                    myDB.getBorrowedMap().insert(book,other_name); //add borrower to borrow map
                                    send(topic,"Taking "+book+" from "+ other_name);
                                }
                            }
                            break;
                        default: // someone sent me a book status, i shall print it!
                            body = result.at(5);
                            std::cout << body << std::endl; //print status
                            break;

                    }

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
                    myDB.setIsShouldTerminate(true);
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
    if(st=="wish")
        return wish;
    if(st=="has")
        return has;

    //for keyboard:
    if(st=="login")
        return LOGIN;
    if(st=="join")
        return JOIN;
    if(st=="exit")
        return EXIT;
    if(st=="add")
        return ADD_BOOK;
    if(st=="borrow")
        return BORROW;
    if(st=="return")
        return RETURN;
    if(st=="status")
        return GENRE;
    if(st=="logout")
        return LOGOUT;

    return -1; //invalid msg header

}

void Protocol::send(std::string topic, std::string body) {
    std::string toSend;
    toSend="SEND\n destination:" + topic + "\n\n" + body + "\n\0";
    handler.sendLine(toSend);
}



void Protocol::process_keyboard(std::string &msg) {

    std::vector<std::string> vector_for_input = Protocol::input_to_vector(msg); //ass method to parse the input
    if (vector_for_input.size() != 0 ){printf("ERROR: invalid input\n");} //test purpose only
    else{
        int actionName = getOpcode(vector_for_input.at(0)); //checks first word in input
        switch(actionName) {
            case LOGIN:
                //assumption: already logged in
                printf("ERROR: Already logged in!\n");//TODO: should be in STOMP format?

                break;
            case JOIN:
                std::string topic = vector_for_input.at(1);
                int receiptId = myDB.getRecIdAndInc();
                int subID = myDB.getSubIdAndInc();

                send("SUBSCRIBE","destination:" +  + "\nid:" + );


                break;
            case EXIT:

                break;
            case ADD_BOOK:

                break;
            case BORROW:

                break;
            case RETURN:

                break;
            case GENRE:

                break;
            case LOGOUT:

                break;

            default: //TODO: should choose what to do in case invalid msg header recieved
                break;
        }


    }


}









