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
Protocol::Protocol(ClientDB& db, ConnectionHandler& handler): myDB(db) , handler(handler) {}

void Protocol::process_server(std::string &msg) {
    printf("inside proccess server\n");
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
        case connected: {
            printf("inside servermsg-connected\n");
            myDB.setIsActive(true);
            std::cout << "Successfully connected to Server!..\n" << std::endl;
            break;
        }
        case message: {
            printf("inside servermsg-message\n");
            boost::split(parse_vec, result.at(5), boost::is_any_of(" ")); //split message body into words
            opcode2 = getOpcode(parse_vec.at(0)); //get first word code
            switch (opcode2) {
                case taking: {
                    if (parse_vec.at(3) == myDB.getMyName()) { //need to give a book
                        book = parse_vec.at(1);
                        boost::split(parse_vec, result.at(3), boost::is_any_of(":")); //get topic
                        topic = parse_vec.at(1);
                        if (myDB.remove_book_from_Inv(book, topic)) {
                            std::cout << "Borrowed " << fix_body(book) << std::endl;
                        }
                    }

                    break;
                }
                case bookstatus: {
                    printf("inside servermsg-bookstatus\n");
                    body = myDB.getMyName() + ":";
                    boost::split(parse_vec, result.at(3), boost::is_any_of(":")); //get topic
                    topic = parse_vec.at(1);
                    books = myDB.get_topic_books(topic);
                    for (std::string book: books)
                        body = body + book + ",";

                    send(topic, body);//send frame
                    break;
                }
                case returning: {
                    printf("inside servermsg-returning\n");
                    if (parse_vec.size() >= 4 &&
                        parse_vec.at(3) == myDB.getMyName()) { //if book is being returned to me
                        book = parse_vec.at(1);
                        boost::split(parse_vec, result.at(3), boost::is_any_of(
                                ":")); //get topic //TODO:check that this wont change book variable value
                        topic = parse_vec.at(1);
                        myDB.add_book_to_Inv(book, topic); //take book back to inv
                    }
                    break;
                }
                //
                default: {
                    printf("inside servermsg-default\n");
                    opcode3 = getOpcode(parse_vec.at(1));
                    book = parse_vec.at(4); //TODO:maybe move back to avoid double code
                    switch (opcode3) {
                        case wish: {
                            printf("inside servermsg-wish\n");

                            if (myDB.inv_contains_book(book, topic)) {
                                send(topic, myDB.getMyName() + " has " + book); //if i have this book send has frame
                            }
                            break;
                        }
                        case has: {
                            printf("inside servermsg-has\n");
                            other_name = parse_vec.at(0);
                            if (myDB.getMyName() != other_name) { //act only if this isn't my message
                                boost::split(parse_vec, result.at(3), boost::is_any_of(
                                        ":")); //get topic //TODO:check that this wont change book variable value
                                topic = parse_vec.at(1);
                                if (myDB.wishList_contains(book)) {
                                    myDB.remove_book_from_wishList(book);
                                    myDB.add_book_to_Inv(book, topic); //add the loaned book
                                    myDB.add_book_to_borrowdMap(book,other_name);
                                    //myDB.getBorrowedMap().insert(std::make_pair(book, other_name)); //add borrower to borrow map
                                    send(topic, "Taking " + fix_body(book) + " from " + other_name);
                                }
                            }
                            break;
                        }
                        default: { // someone sent me a book status, i shall print it!
                            printf("inside servermsg-double default - print status\n");
                            body = result.at(5);
                            std::cout << fix_body(body) << std::endl; //print status
                            break;
                        }

                    }
                }

            }

            break;
        }
        case receipt: {
            printf("inside servermsg-receipt\n");
            boost::split(parse_vec, result.at(1), boost::is_any_of(":"));
            receipt_num = stoi(parse_vec.at(1)); //get receipt number
            mission_info  = myDB.get_receipt_info(receipt_num);
            opcode2 = getOpcode(mission_info.at(0)); //get the type of my receipt message
            switch (opcode2) {
                case disconnect: {
                    printf("inside servermsg-disconnect\n");
                    myDB.setIsActive(false); //TODO:ofer: check if valid change (here is where we close socket!)
                    myDB.setIsShouldTerminate(true);
                    handler.close(); //close the socket
                    std::cout << "Successful logout from Server!..\n" << std::endl;
                    break;
                }
                case subscribe: {
                    printf("inside servermsg-subscribe\n");
                    std::string theTopic = mission_info.at(2);
                    myDB.add_to_myTopics(mission_info.at(1),stoi(theTopic));

                    if(myDB.is_inv_contains_topic(theTopic)){ //put topic in inv if absent
                        myDB.add_topic_to_inv(theTopic);
                    }

                    std::cout << "Joined club "
                              << mission_info.at(1) << std::endl;
                    break;
                }
                case unsubscribe: {
                    printf("inside servermsg-unsubscribe\n");
                    myDB.remove_from_myTopics(mission_info.at(1));
                    std::cout << "Exited club "
                              << mission_info.at(1) << std::endl;
                    break;
                }
                default:{
                    std::cout <<"processing default case:" + result.at(5) << std::endl; //print what happened, for testing purposes
                    break;
                }
            }
            break;
        }
        case error: {
            std::cout << "Error occured:\n"
                      << msg << std::endl;
            myDB.setIsShouldTerminate(true);
            myDB.setIsActive(false);
            handler.close();
            break;
        }
        default: { //TODO: should choose what to do in case invalid msg header recieved

            break;
        }
    }

}


void Protocol::process_keyboard(std::string &msg) {

    ////////////////
    //parameters to prove the method's flow:
    std::string topic ;
    int receiptId ;
    int subID ;
    std::string body;
    std::vector<std::string> tmpVector;

    std::string loaner_name;
    std::vector<std::string> receipt_vec;
    std::string send_string;
    /////////////

    std::vector<std::string> vector_for_input = Protocol::input_to_vector(msg); //ass method to parse the input
    if (vector_for_input.empty() ){printf("ERROR: invalid input\n");} //test purpose only
    else{
        int actionName = getOpcode(vector_for_input.at(0)); //checks first word in input
        switch(actionName) {
            case LOGIN: {
                //assumption: already logged in
                printf("ERROR: Already logged in!\n");//TODO: should be in STOMP format?

                break;
            }
            case JOIN: {
                topic = vector_for_input.at(1);
                receiptId = myDB.getRecIdAndInc();
                subID = myDB.getSubIdAndInc();
                tmpVector.push_back("SUBSCRIBE");
                tmpVector.push_back(vector_for_input.at(1));
                tmpVector.push_back(std::to_string(subID));
                //insert to maps:
                myDB.add_receipt(receiptId, tmpVector);

                send_stomp_frame("SUBSCRIBE", "destination:" + topic + "\nid:" + std::to_string(subID) + "\nreceipt:" +
                                              std::to_string(receiptId));

                break;
            }
            case EXIT: {
                subID = myDB.get_subscription_id(vector_for_input.at(1));
                //subID = myDB.getMyTopics().at(vector_for_input.at(1));
                receiptId = myDB.getRecIdAndInc();

                tmpVector.push_back("UNSUBSCRIBE");
                tmpVector.push_back(vector_for_input.at(1));
                tmpVector.push_back(std::to_string(subID));
                myDB.add_receipt(receiptId,tmpVector);

                send_stomp_frame("UNSUBSCRIBE",
                                 "id:" + std::to_string(subID) + "\nreceipt:" + std::to_string(receiptId));

                break;
            }
            case ADD_BOOK: {
                vector_for_input.at(2) = unify_book_name(vector_for_input); //get unified by - book name //TODO: WHAT HAPPENS HERE?
                std::string theTopic = vector_for_input.at(1); // the topic of the book

                if(myDB.is_inv_contains_topic(theTopic)){ //put topic in inv if absent
                    myDB.add_topic_to_inv(theTopic);
                }

                myDB.add_book_to_Inv(vector_for_input.at(2), theTopic);
                send(vector_for_input.at(1), myDB.getMyName() + " has added the book " + vector_for_input.at(2));
                break;
            }
            case BORROW:{

                myDB.add_book_to_wishList(vector_for_input.at(2));
                send(vector_for_input.at(1),myDB.getMyName()+" wish to borrow "+vector_for_input.at(2));
                break;
            }
            case RETURN: {
                myDB.remove_book_from_Inv(vector_for_input.at(2), vector_for_input.at(1)); //remove from my inv
                loaner_name = myDB.get_loaner_name(vector_for_input.at(2));
                //loaner_name = myDB.getBorrowedMap().at(vector_for_input.at(2));
                myDB.remove_from_borrowdMap(vector_for_input.at(2));//remove borrower from borrow map
                send(vector_for_input.at(1), "Returning " + vector_for_input.at(2) + " to " + loaner_name);
                break;
            }
            case GENRE: {
                send(vector_for_input.at(1), "book status");
                break;
            }
            case LOGOUT: {
                receipt_vec.push_back("DISCONNECT");
                receiptId = myDB.getRecIdAndInc();
                myDB.add_receipt(receiptId,receipt_vec);
                send_stomp_frame((std::string &) "DISCONNECT", "receipt:" + std::to_string(receiptId) + "\n\n");
                break;
            }
            default: { //TODO: should choose what to do in case invalid msg header recieved
                printf("invalid keyboard input,try again stupid\n");
                break;
            }
        }


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
    if(st=="SUBSCRIBE")
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
    toSend="SEND\n destination:" + topic + "\n\n" + body + "\n"+'\0';
    handler.sendLine(toSend);
}
void Protocol::send_stomp_frame(std::string header, std::string body) {
    std::string toSend;
    toSend = header+"\n\n"+body+ "\n"+'\0';
    handler.sendLine(toSend);
}






std::string Protocol::unify_book_name(std::vector<std::string> &vec) {
    std::string output=vec.at(2);
    for(int i =3; i<vec.size();i++){
        output = output +"-"+vec.at(i);
    }
    return output;
}

std::string Protocol::fix_body(std::string &body) {
    std::string toPrint;
    for(char c: body){
        if(c=='-')
            toPrint+=' ';
        else
            toPrint+=c;
    }


}


















