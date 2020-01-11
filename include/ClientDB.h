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

    const std::unordered_map<int, std::vector<std::string>> &getReceiptMap() const;

    void setIsActive(bool isActive);

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

//


private:
    bool isShouldTerminate;
    std::string myName;
    bool isActive;
    std::unordered_map<std::string,std::vector<std::string>> myInventory; //1st = topic , 2nd =
    std::unordered_map<std::string,std::string> borrowedMap; // key = book, val = name of loaner
    std::unordered_map<int,std::vector<std::string>> receiptMap; //key=receipt num, val=vector of important info (messageType,topic, etc....)
    Protocol protocol; //TODO:Ofer: why?
    int receiptnumcounter; //this wil be a unique number of each receipt
    int subscriptionid; //



};


#endif //SPL_NET_CLIENT_CLIENTDB_H
