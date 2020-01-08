#include <stdlib.h>
#include "../include/connectionHandler.h"
#include "../include/ClientDB.h"

std::vector<std::string> input_to_vector(const std::string& basicString);

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main () {
//    if (argc < 3) {
//        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
//        return -1;
//    }
//    std::string host = argv[1];
//    short port = atoi(argv[2]);
//
//    ConnectionHandler connectionHandler(host, port);  //TODO: Alon not relevant for our ass

    const short bufsize = 1024;
    char buf[bufsize];
    std::cin.getline(buf, bufsize);
    std::string line(buf);
    int len =line.length();

    //handle the login (first command):
    std::string input_string;
    getline(std::cin,input_string);
    std::vector<std::string> vector_for_input =  input_to_vector(input_string); //ass method to parse the input


    //TODO: ALON 8.1 2130 : assuming login is first input, not sure if right
    std::string password =
    std::string host = "";
    std::string tmpPort = "";
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
    //at this









//--------------------------------------------------------------------------------------------



    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
	
	//From here we will see the rest of the ehco client implementation:
    while (1) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
		std::string line(buf);
		int len=line.length();
        if (!connectionHandler.sendLine(line)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
		// connectionHandler.sendLine(line) appends '\n' to the message. Therefor we send len+1 bytes.
        std::cout << "Sent " << len+1 << " bytes to server" << std::endl;

 
        // We can use one of three options to read data from the server:
        // 1. Read a fixed number of characters
        // 2. Read a line (up to the newline character using the getline() buffered reader
        // 3. Read up to the null character
        std::string answer;
        // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
        // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
        if (!connectionHandler.getLine(answer)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        
		len=answer.length();
		// A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
		// we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
        answer.resize(len-1);
        std::cout << "Reply: " << answer << " " << len << " bytes " << std::endl << std::endl;
        if (answer == "bye") {
            std::cout << "Exiting...\n" << std::endl;
            break;
        }
    }
    return 0;
}

std::vector<std::string> input_to_vector(const std::string& str) {
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

