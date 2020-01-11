#include <stdlib.h>
#include <thread>
#include "../include/connectionHandler.h"
#include "../include/ClientDB.h"
#include "../include/ServerListenerTask.h"
#include "../include/Protocol.h"
#include "../include/KeyboardListenerTask.h"

std::vector<std::string> input_to_vector(const std::string& basicString);

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main () {
    while (1) {
        ClientDB clientDb;
        Protocol aProtocol = Protocol(); //TODO: ALON 11.12 1610 is right syntax?
        clientDb.setIsActive(false);

        //ConnectionHandler* connectionHandler;

        //handle the login (first command):
        std::string input_string;
        getline(std::cin, input_string);
        std::vector<std::string> vector_for_input = Protocol::input_to_vector(input_string); //ass method to parse the input
        if (vector_for_input.size() != 0 || vector_for_input.at(0) != "logout"){ break;} //TODO: ALON not sure if right, CHECK
        while (vector_for_input.size() == 0 || vector_for_input.at(0) != "login") {
            printf("ERROR: user is not logged in yet \ntry again: \n");
            getline(std::cin, input_string);
            vector_for_input = input_to_vector(input_string); //assistant method to parse the input to vector
        } //TODO: committed only to test something

        //if (vector_for_input.at(0) == "login"){
//        if (!clientDb.getIsActive()){
//            printf("ERROR: user already logged\n"); //TODO: need to be replaced to stomp format!
//        } else{

        //at this point, a login command is received
        std::string host;
        std::string tmpPort;
        short port;
        int i = 0;
        while (vector_for_input.at(1).at(i) != ':') {
            host += vector_for_input.at(1).at(i);
            i++;
        }
        i++;
        while (i < vector_for_input.at(1).length()) {
            tmpPort += vector_for_input.at(1).at(i);
            i++;
        }
        port = (short) std::stoi(tmpPort); //converting the string to int, and casted to short (assump: valid input)
        std::string myName = vector_for_input.at(2);
        std::string password = vector_for_input.at(3);
        clientDb.setMyName(myName);


        //creating connection with server:
        ConnectionHandler connectionHandler(host, port);
        if (!connectionHandler.connect()) {
            printf("Could not connect to server\n");  //TODO: need to be replaced to stomp format!
        } else {
            //Thread (new Task(connectionHandler, name, password));
            ServerListenerTask serverListenerTask(connectionHandler, myName, clientDb);
            KeyboardListenerTask keyBoardListenerTask(&connectionHandler, myName, clientDb);
            std::thread th1(std::ref(serverListenerTask)); //TODO: Check if ok

            aProtocol.send("CONNECT", "version:1.2\n\n^@"); //TODO: check if ^@ or \0
            std::thread th2(std::ref(keyBoardListenerTask)); //TODO: Check if ok

            //now client is up and running. waits until logged out:
            th1.join();
            th2.join();
        }


        //}
        //}
















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

