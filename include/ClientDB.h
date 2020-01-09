//
// Created by michalon@wincs.cs.bgu.ac.il on 08/01/2020.
//

#ifndef SPL_NET_CLIENT_CLIENTDB_H
#define SPL_NET_CLIENT_CLIENTDB_H


#include <vector>
#include <string>
#include <unordered_map>
#include "Protocol.h"


class ClientDB {
public:


    void initialize(std::string& name);

    std::vector<std::string> getBooksOfTopic;

    bool getIsActive() const;

    const std::vector<std::string> &getGetBooksOfTopic() const;

    void setGetBooksOfTopic(const std::vector<std::string> &getBooksOfTopic);

    const std::string &getMyName() const;

    void setMyName(const std::string &myName);

    const std::unordered_map<std::string, std::vector<std::string>> &getMyInventory() const;

    void setMyInventory(const std::unordered_map<std::string, std::vector<std::string>> &myInventory);

    const std::unordered_map<std::string, std::string> &getBorrowedMap() const;

    void setBorrowedMap(const std::unordered_map<std::string, std::string> &borrowedMap);

    void setIsActive(bool isActive);


private:
    std::string myName;
    bool isActive;
    std::unordered_map<std::string,std::vector<std::string>> myInventory;
    std::unordered_map<std::string,std::string> borrowedMap; // key = book, val = name of loaner
    Protocol protocol;


};


#endif //SPL_NET_CLIENT_CLIENTDB_H
