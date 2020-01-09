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
