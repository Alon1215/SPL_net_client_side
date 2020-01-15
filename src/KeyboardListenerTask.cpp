//
// Created by michalon@wincs.cs.bgu.ac.il on 11/01/2020.
//

#include "../include/KeyboardListenerTask.h"
KeyboardListenerTask::KeyboardListenerTask(ConnectionHandler &handler, std::string name, ClientDB &db,Protocol &protocol) : handler(handler), name(name), db(db),protocol(protocol){}

void KeyboardListenerTask::operator()() {

    //aProtocol.send("CONNECT", "version:1.2\n\n^@"); //TODO: check if ^@ or \0
    while(!db.getIsShouldTerminate1()){ //TODO: change to bool in db
        printf("Keyboard  task operating\n");
        std::string input_string;
        getline(std::cin, input_string);
        if (!db.getIsActive()){
            printf("ERROR: not logged in yet!\n");  //TODO: should be in STOMP format?
        }

        else{
            protocol.process_keyboard(input_string);
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