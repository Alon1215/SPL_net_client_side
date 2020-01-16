#include <stdlib.h>
#include <thread>
#include "../include/connectionHandler.h"
#include "../include/ClientDB.h"
#include "../include/ServerListenerTask.h"
#include "../include/Protocol.h"
#include "../include/KeyboardListenerTask.h"

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main () {
    while (1) {
        printf("Welcome to the book club!\nstart by log in:\n");


        //handle the login (first command):
        std::string input_string;
        getline(std::cin, input_string);
        std::vector<std::string> vector_for_input = Protocol::input_to_vector(input_string); //ass method to parse the input

        if (!vector_for_input.empty() && vector_for_input.at(0) == "bye"){ break;} //exit program command

        while (vector_for_input.empty() || vector_for_input.at(0) != "login") {
            printf("ERROR: user is not logged in yet \ninput message: %s \ntry again: \n",input_string.c_str());
            getline(std::cin, input_string);
            vector_for_input = Protocol::input_to_vector(input_string); //assistant method to parse the input to vector
        } //TODO: committed only to test something


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


        //creating connection with server:
        ConnectionHandler connectionHandler(host, port);

        ClientDB clientDb;
        clientDb.setMyName(myName);
        Protocol aProtocol(clientDb,connectionHandler);
        if (!connectionHandler.connect()) {
            printf("Could not connect to server\n");  //TODO: need to be replaced to stomp format!
        } else {

            ServerListenerTask serverListenerTask(connectionHandler, myName, clientDb,aProtocol);
            KeyboardListenerTask keyBoardListenerTask(connectionHandler, myName, clientDb,aProtocol);
            std::thread th1(std::ref(serverListenerTask)); //TODO: Check if ok

            aProtocol.send_stomp_frame("CONNECT", "accept-version:1.2 \n"
                                      "host:stomp.cs.bgu.ac.il \n"
                                      "login:"+myName+"\n"
                                                      "password:" += password);
            printf("Sent Connect frame to server\n");
            std::thread th2(std::ref(keyBoardListenerTask)); //TODO: Check if ok



            //now client is up and running. waits until logged out:
            th1.join();
            th2.join();
        }
















    }
//--------------------------------------------------------------------------------------------

    return 0;
}


