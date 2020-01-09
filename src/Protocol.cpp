//
// Created by mosesofe@wincs.cs.bgu.ac.il on 09/01/2020.
//

#include <boost/asio/ip/tcp.hpp>
#include "../include/Protocol.h"
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>

enum string_code{
    elogin
};
void Protocol::process_keyboard(std::string  &msg) {

}

void Protocol::process_server(std::string &msg) {
    std::vector<std::string> result;
    boost::split(result, msg, boost::is_any_of("\n"));
    std::string Opcode = result.at(0);
    switch(elogin)



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
