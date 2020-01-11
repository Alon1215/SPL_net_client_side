//
// Created by michalon@wincs.cs.bgu.ac.il on 11/01/2020.
//

#include "../include/KeyboardListenerTask.h"
KeyboardListenerTask::KeyboardListenerTask(ConnectionHandler handler, std::string name, ClientDB &db) (ConnectionHandler *handler, std::string name, ClientDB &db): handler(&handler), name(name), db(db) {}

void KeyboardListenerTask::operator()() {
    Protocol &aProtocol = db.getProtocol();
    bool shouldTerminate = db.getIsShouldTerminate1();

    //aProtocol.send("CONNECT", "version:1.2\n\n^@"); //TODO: check if ^@ or \0
    while(!shouldTerminate){ //TODO: change to bool in db
        std::string input_string;
        getline(std::cin, input_string);
        if (!db.getIsActive()){printf("ERROR: not logged in yet!\n"); }//TODO: should be in STOMP format?

        else{
            aProtocol.process_keyboard(input_string);
        }

//
//        std::vector<std::string> vector_for_input = Protocol::input_to_vector(input_string); //ass method to parse the input
//        if (vector_for_input.size() != 0 ){printf("ERROR: invalid input\n");} //test purpose only
//        else{
//            aProtocol.process_keyboard(input_string)
//
//        }


    }

}