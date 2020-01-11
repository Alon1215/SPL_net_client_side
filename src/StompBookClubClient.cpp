#include <stdlib.h>
#include <thread>
#include "../include/connectionHandler.h"
#include "../include/ClientDB.h"
#include "../include/ServerListenerTask.h"
#include "../include/Protocol.h"

std::vector<std::string> input_to_vector(const std::string& basicString);

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main () {

    const short bufsize = 1024;
    char buf[bufsize];
    ClientDB clientDb;
    Protocol *aProtocol = new Protocol;
    //ConnectionHandler* connectionHandler;

//    std::cin.getline(buf, bufsize);
//    std::string line(buf);
//    int len =line.length();//TODO: is relevant? not to delete at the moment

    //handle the login (first command):
    std::string input_string;
    getline(std::cin,input_string);
    std::vector<std::string> vector_for_input = Protocol::input_to_vector(input_string); //ass method to parse the input

//    while (vector_for_input.size() == 0 || vector_for_input.at(0) != "login"){
//        printf("ERROR: user is not logged in yet \ntry again: \n");
//        getline(std::cin,input_string);
//        vector_for_input = input_to_vector(input_string); //assistant method to parse the input to vector
//     } //TODO: committed only to test something

    if (vector_for_input.at(0) == "login"){
        if (!clientDb.getIsActive()){
//            "ERROR\n"
//            "receipt-id: message-12345\n"
//            "message: malformed frame receivedThe message:\n"
//            "-----\n"
//            "MESSAGE\n"
//            "destined:/queue/a\n"
//            "receipt: message-12345\n"
//            "Hello queue a!\n"
//            "-----\n"
//            "Did not contain a destination header, which is REQUIRED for message propagation.^@";
            printf("ERROR: user already logged\n"); //TODO: need to be replaced to stomp format!
        } else{

            //at this point, a login command is received
            std::string host;
            std::string tmpPort;
            short port;
            int i = 0;
            while (vector_for_input.at(1).at(i) != ':'){
                host += vector_for_input.at(1).at(i);
                i++;
            }
            i++;
            while (i< vector_for_input.at(1).length()){
                tmpPort += vector_for_input.at(1).at(i);
                i++;
            }
            port = (short) std::stoi(tmpPort); //converting the string to int, and casted to short (assump: valid input)
            std::string myName = vector_for_input.at(2);
            std::string password = vector_for_input.at(3);


            //creating connection with server:
            ConnectionHandler connectionHandler(host, port);
            if (!connectionHandler.connect()) {
                //std::cerr << "Cannot connect to " << host << ":" << port << std::endl; //TODO: not sure if relevant
                printf("Could not connect to server\n");  //TODO: need to be replaced to stomp format!
            }
            //Thread (new Task(connectionHandler, name, password));
            ServerListenerTask serverListenerTask(&connectionHandler,myName,clientDb);
            std::thread th1(std::ref(serverListenerTask)); //TODO: Check if ok

        }
    }

















//--------------------------------------------------------------------------------------------

    return 0;
}

//std::vector<std::string> input_to_vector(const std::string& str) {
//    std::string word = "";
//    std::vector<std::string> output;
//    for (auto x : str)
//    {
//        if (x == ' ')
//        {
//            std::string newWord = word;
//            output.push_back(word);
//            word = "";
//        }
//        else
//        {
//            word = word + x;
//        }
//    }
//    output.push_back(word);
//    return output;
//}

//
//static std::vector<std::string> input_to_vector(const std::string& str) {
//    std::string word = "";
//    std::vector<std::string> output;
//    for (auto x : str)
//    {
//        if (x == ' ')
//        {
//            std::string newWord = word;
//            output.push_back(word);
//            word = "";
//        }
//        else
//        {
//            word = word + x;
//        }
//    }
//    output.push_back(word);
//    return output;
//}

