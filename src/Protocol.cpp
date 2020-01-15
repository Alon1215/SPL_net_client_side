//
// Created by mosesofe@wincs.cs.bgu.ac.il on 09/01/2020.
//

#include <boost/asio/ip/tcp.hpp>
#include "../include/Protocol.h"
#include "../include/connectionHandler.h"
#include <bits/stdc++.h>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>

enum string_code{
    connected,receipt,message,error,
    disconnect,subscribe,unsubscribe, //receipt cases
    returning, bookstatus,taking,has,wish,  //message cases
    LOGIN, JOIN, EXIT, ADD_BOOK, BORROW, RETURN, STATUS, LOGOUT //for keyboard

};
Protocol::Protocol(ClientDB& db, ConnectionHandler& handler): myDB(db) , handler(handler) {}

void Protocol::process_server(std::string &msg) {

    //variables to be used by cases, in order to process msg from server:
    //-----
    printf("inside proccess server\n");
    std::vector<std::string> result; //vector of all lines in input msg
    std::vector<std::string> parse_vec;
    std::vector<std::string> receipt_info;
    std::vector<std::string> books;
    std::string book;
    std::string topic;
    std::string body;
    int receipt_num;
    std::string other_name;
    boost::split(result, msg, boost::is_any_of("\n"));

    //three parameters parsed from result, sorted by importance order:
    int param_result_1 = getOpcode(result.at(0)); //name of main case to process
    int param_result_2;
    int param_result_3;
    //------

    switch(param_result_1) {
        case connected: {
            std::cout << msg+"\n\n" << std::endl;
            printf("inside servermsg-connected\n");
            myDB.setIsActive(true);
            std::cout << "Successfully connected to Server!..\n" << std::endl;
            break;
        }
        case message: {
            printf("inside servermsg-message\n");
            boost::split(parse_vec, result.at(5), boost::is_any_of(" ")); //split message body into words
            param_result_2 = getOpcode(parse_vec.at(0)); //get first word code
            std::cout << msg+"\n\n" << std::endl;
            switch (param_result_2) {
                case taking: {
                    if (parse_vec.at(3) == myDB.getMyName()) { //need to give a book
                        book = parse_vec.at(1);
                        boost::split(parse_vec, result.at(3), boost::is_any_of(":")); //get topic
                        topic = parse_vec.at(1);
                        if (myDB.remove_book_from_Inv(book, topic)) {
                            std::cout << "Borrowed" << fix_body(book)+"\n\n" << std::endl; //TODO: print for testing
                        }
                    }

                    break;
                }
                case bookstatus: { //this is the case where you send your book status to all subscribers in genre
                    printf("inside servermsg-bookstatus\n");
                    body = myDB.getMyName() + ":";
                    boost::split(parse_vec, result.at(3), boost::is_any_of(":")); //get topic
                    topic = parse_vec.at(1);
                    books = myDB.get_topic_books(topic);
                    for (int i=0;i<books.size();i++) {
                        body = body + fix_book_name(books.at(i)); // if it's not the last one
                        if(i != books.size()-1)
                            body += ", ";
                    }

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
                    if(parse_vec.size()>1)
                        param_result_3 = getOpcode(parse_vec.at(1));
                    else
                        param_result_3 = -1; //TODO: Ofer: added because in case of status(printing it) (or maybe invalid msg?) parse_vec will have only 1 cell, check that it won't cause bugs
                    switch (param_result_3) {
                        case wish: {
                            std::cout<<result.at(0)+'\n'+result.at(1)+'\n'+result.at(2)+'\n'+result.at(3)+"\n\n"+fix_body(result.at(5))+'\n'+'\n'<<std::endl;
                            book = parse_vec.at(4); //TODO:maybe move back to avoid double code
                            std::cout<<result.at(0)+'\n'+result.at(1)+'\n'+result.at(2)+'\n'+result.at(3)+'\n'+fix_body(result.at(5))<<std::endl;
                            boost::split(parse_vec, result.at(3), boost::is_any_of(":"));
                            topic = parse_vec.at(1);
                            if (myDB.inv_contains_book(book, topic)) {
                                send(topic, myDB.getMyName() + " has " + book); //if i have this book send has frame
                            }
                            break;
                        }
                        case has: {
                            if(parse_vec.at(2)=="added"){
                                std::cout<<result.at(0)+'\n'+result.at(1)+'\n'+result.at(2)+'\n'+result.at(3)+"\n\n"+fix_body(result.at(5))+'\n'+'\n'<<std::endl;
                            }
                            else {
                                book = parse_vec.at(2); //TODO:maybe move back to avoid double code
                                printf("inside servermsg-has\n");
                                other_name = parse_vec.at(0);
                                if (myDB.getMyName() != other_name) { //act only if this isn't my message
                                    boost::split(parse_vec, result.at(3), boost::is_any_of(
                                            ":")); //get topic //TODO:check that this wont change book variable value
                                    topic = parse_vec.at(1);
                                    if (myDB.wishList_contains(book)) {
                                        myDB.remove_book_from_wishList(book);
                                        myDB.add_book_to_Inv(book, topic); //add the loaned book
                                        myDB.add_book_to_borrowdMap(book, other_name);
                                        //myDB.getBorrowedMap().insert(std::make_pair(book, other_name)); //add borrower to borrow map
                                        send(topic, "Taking " + fix_body(book) + " from " + other_name);
                                    }
                                }
                            }
                            break;
                        }

                        default:{ //print msg
                            std::cout << msg+"\n\n" << std::endl;
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
            receipt_info  = myDB.get_receipt_info(receipt_num);
            param_result_2 = getOpcode(receipt_info.at(0)); //get the type of my receipt message
            switch (param_result_2) {
                case disconnect: {
                    printf("inside servermsg-disconnect\n");
                    myDB.setIsActive(false); //TODO:ofer: check if valid change (here is where we close socket!)
                    myDB.setIsShouldTerminate(true);
                    handler.close(); //close the socket
                    std::cout << "Successful logout from Server!..\n" << std::endl;
                    break;
                }
                case subscribe: {
                    std::cout << msg+"\n\n" << std::endl;
                    printf("inside servermsg-subscribe\n");
                    std::string theTopic = receipt_info.at(2);
                    myDB.add_to_myTopics(receipt_info.at(1), stoi(theTopic));

                    if(!myDB.is_inv_contains_topic(theTopic)){ //put topic in inv if absent
                        myDB.add_topic_to_inv(theTopic);
                    }

                    std::cout << "Joined club "
                              << receipt_info.at(1) << std::endl;
                    break;
                }
                case unsubscribe: {
                    std::cout << msg+"\n\n" << std::endl;
                    printf("inside servermsg-unsubscribe\n");
                    myDB.remove_from_myTopics(receipt_info.at(1));
                    std::cout << "Exited club "
                              << receipt_info.at(1) << std::endl;
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

    //-----
    //parameters to prove the method's flow:
    std::string topic ;
    int receiptId ;
    int subID ;
    std::string body;
    std::vector<std::string> tmpVector;

    std::string loaner_name;
    std::vector<std::string> receipt_vec;
    std::string send_string;
    //-----

    std::vector<std::string> vector_for_input = Protocol::input_to_vector(msg); //ass method to parse the input
    if (vector_for_input.empty() ){printf("ERROR: invalid input\n");} //test purpose only
    else{
        int actionName = getOpcode(vector_for_input.at(0)); //checks first word in input
        switch(actionName) {
            case LOGIN: {
                //assumption: already logged in
                printf("ERROR: Already logged in!\n");//TODO: maybe should enable sending login msg inorder to get error from server like in instructions


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
                std::string bookName = unify_book_name(vector_for_input); //get unified by - book name //TODO: WHAT HAPPENS HERE?
                std::string theTopic = vector_for_input.at(1); // the topic of the book

                if(!myDB.is_inv_contains_topic(theTopic)){ //put topic in inv if absent
                    myDB.add_topic_to_inv(theTopic);
                }

                myDB.add_book_to_Inv(bookName, theTopic);
                send(vector_for_input.at(1), myDB.getMyName() + " has added the book " + bookName);
                break;
            }
            case BORROW:{
                std::string bookName = unify_book_name(vector_for_input);
                myDB.add_book_to_wishList(bookName);
                send(vector_for_input.at(1),myDB.getMyName()+" wish to borrow "+bookName);
                break;
            }
            case RETURN: {
                if(vector_for_input.size()<3)
                    std::cout << "invalid input from keyboard at return case\n" << std::endl;
                else{
                    myDB.remove_book_from_Inv(vector_for_input.at(2), vector_for_input.at(1)); //remove from my inv
                    loaner_name = myDB.get_loaner_name(vector_for_input.at(2));
                    //loaner_name = myDB.getBorrowedMap().at(vector_for_input.at(2));
                    myDB.remove_from_borrowdMap(vector_for_input.at(2));//remove borrower from borrow map
                    send(vector_for_input.at(1), "Returning " + vector_for_input.at(2) + " to " + loaner_name);
                }
                break;
            }
            case STATUS: {
                send(vector_for_input.at(1), "Book status");
                break;
            }
            case LOGOUT: {
                receipt_vec.push_back("DISCONNECT");
                receiptId = myDB.getRecIdAndInc();
                myDB.add_receipt(receiptId,receipt_vec);
                send_stomp_frame( "DISCONNECT", "receipt:" + std::to_string(receiptId) + "\n\n");
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
    if(st=="Book")
        return bookstatus;
    if(st=="Returning")
        return returning;
    if(st=="Taking")
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
        return STATUS;
    if(st=="logout")
        return LOGOUT;

    return -1; //invalid msg header

}

void Protocol::send(std::string topic, std::string body) {
    std::string toSend;
    toSend="SEND\ndestination:" + topic + "\n\n" + body + "\n";
    handler.sendLine(toSend);
}
void Protocol::send_stomp_frame(std::string header, std::string body) {
    std::string toSend;
    toSend = header+"\n"+body+ "\n\n";
    handler.sendLine(toSend);
}






std::string Protocol::unify_book_name(std::vector<std::string> &vec) {
    std::string output=vec.at(2);
    for(int i =3; i<vec.size();i++){
        output = output +"-"+vec.at(i);
    }
    return output;
}
//std::string Protocol::unify_book_name_borrow(std::vector<std::string> &vec) {
//    //std::string output = vec.at()
//}




std::string Protocol::fix_body(std::string &body) {
    std::string toPrint;
    for(char c: body){
        if(c=='-')
            toPrint+=' ';
        else
            toPrint+=c;
    }
    return toPrint;
}

std::string Protocol::fix_book_name(std::string book) {
    std::string fixed_book;
    for(char c:book){
        if(c=='-')
            fixed_book+=' ';
        else
            fixed_book+=c;
    }
    return fixed_book;
}




















