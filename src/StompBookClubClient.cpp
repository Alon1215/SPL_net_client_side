#include <stdlib.h>
#include <thread>
#include "../include/connectionHandler.h"
#include "../include/ClientDB.h"
#include "../include/ServerListenerTask.h"
#include "../include/Protocol.h"

void obtainArgsForClient(std::vector<std::string> &vector_for_input, std::string &tmpPort, std::string &host, short &port,
                         std::string &myName, std::string &password);

void keyboardRunLoop(const ClientDB &clientDb, Protocol &aProtocol, std::string &input_string, bool &finishRun);

void obtainParsedInput(std::string &input_string, std::vector<std::string> &vector_for_input);

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main () {
    std::string input_string;
    bool finishRun =false;

    while (!finishRun) {
        printf("Welcome to the book club!\nstart by log in:\n");


        //handle the login (first command):

        std::vector<std::string> vector_for_input;
        obtainParsedInput(input_string, vector_for_input); //fill the vector with input parsed

        //in case the command wasn't login:

        while (vector_for_input.empty() || vector_for_input.at(0) != "login") {
            printf("ERROR: user is not logged in yet \ninput message: %s \ntry again: \n",input_string.c_str());
            getline(std::cin, input_string);
            vector_for_input = Protocol::input_to_vector(input_string); //assistant method to parse the input to vector
        }


        //at this point, a login command is received:

        std::string host;
        std::string tmpPort;
        short port;
        std::string myName;
        std::string password;

        obtainArgsForClient(vector_for_input, tmpPort, host, port, myName, password);//update the parameters
        //------------------------

        //init all shared data & creating connection with server:
        ConnectionHandler connectionHandler(host, port);
        ClientDB clientDb;
        clientDb.setMyName(myName);
        Protocol aProtocol(clientDb,connectionHandler);

        //connect to server:
        if (!connectionHandler.connect()) {
            printf("Could not connect to server\n");
        } else {

            ServerListenerTask serverListenerTask(connectionHandler, myName, clientDb,aProtocol);
            std::thread th1(std::ref(serverListenerTask));

            //send first connect frame:

            aProtocol.send_stomp_frame("CONNECT", "accept-version:1.2 \n"
                                      "host:stomp.cs.bgu.ac.il \n"
                                      "login:"+myName+"\n"
                                                      "passcode:" += password);
            printf("Sent Connect frame to server\n");

            //enters the keyboard listening loop:
            keyboardRunLoop(clientDb, aProtocol, input_string, finishRun);


            //now client is up and running. waits until logged out:
            th1.join();
        }
















    }
//--------------------------------------------------------------------------------------------

    return 0;
}

void obtainParsedInput(std::string &input_string, std::vector<std::string> &vector_for_input) {
    if (input_string.empty()) //else - input already received (handle case after log out)
        getline(std::cin, input_string);//ass method to parse the input
    vector_for_input= Protocol::input_to_vector(input_string);

}

void keyboardRunLoop(const ClientDB &clientDb, Protocol &aProtocol, std::string &input_string, bool &finishRun) {
    while(!clientDb.getIsShouldTerminate1()){

        getline(std::cin, input_string);

        if(clientDb.getIsShouldTerminate1()) { //handle
            if (input_string == "bye") {
                finishRun = true;
                printf("Client is out, see you next time!");
            }
            break;
        }

        if (!clientDb.getIsActive()){
            printf("ERROR: not logged in yet!\n");
        } else{
            aProtocol.process_keyboard(input_string);
        }

    }
}

void obtainArgsForClient(std::vector<std::string> &vector_for_input, std::string &tmpPort, std::string &host, short &port,
                         std::string &myName, std::string &password) {
    myName= vector_for_input.at(2);
    password= vector_for_input.at(3);
    int i = 0;
    while (vector_for_input.at(1).at(i) != ':') {
        host += vector_for_input.at(1).at(i);
        i++;
    }
    i++;
    while (i < (signed) vector_for_input.at(1).length()) {
        tmpPort += vector_for_input.at(1).at(i);
        i++;
    }

    port = (short) std::stoi(tmpPort); //converting the string to int, and casted to short (assump: valid input)
}


