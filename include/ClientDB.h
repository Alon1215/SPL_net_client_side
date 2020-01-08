//
// Created by michalon@wincs.cs.bgu.ac.il on 08/01/2020.
//

#ifndef SPL_NET_CLIENT_CLIENTDB_H
#define SPL_NET_CLIENT_CLIENTDB_H


#include <vector>
#include <string>
#include <unordered_map>


class ClientDB {
public:
    ClientDB(std::vector<std::string> &getBooksOfTopic, const std::string &myName,
             std::unordered_map<std::string, std::vector<std::string>> &myInventory,
             std::unordered_map<std::string, std::string> &borrowedMap);

    std::vector<std::string> getBooksOfTopic;


private:
    std::string myName;
    std::unordered_map<std::string,std::vector<std::string>> myInventory;
    std::unordered_map<std::string,std::string> borrowedMap; // key = book, val = name of loaner

};


#endif //SPL_NET_CLIENT_CLIENTDB_H
