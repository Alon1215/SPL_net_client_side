//
// Created by michalon@wincs.cs.bgu.ac.il on 11/01/2020.
//

#include "../include/KeyboardListenerTask.h"
KeyboardListenerTask::KeyboardListenerTask(ConnectionHandler &handler, std::string name, ClientDB &db,Protocol &protocol) : handler(handler), name(name), db(db),protocol(protocol){}

void KeyboardListenerTask::operator()() {

    while(!db.getIsShouldTerminate1()){ //TODO: change to bool in db
        printf("Keyboard  task operating\n");
        std::string input_string;
        getline(std::cin, input_string);
        if(input_string == "bye bye") { break;} //after logout, reset keyboard for next session //TODO: ALON 16.1 1330 change the string here & function
        if (!db.getIsActive()){
            printf("ERROR: not logged in yet!\n");  //TODO: should be in STOMP format?
        } else{
            protocol.process_keyboard(input_string);
        }

    }

}