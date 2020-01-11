//
// Created by michalon@wincs.cs.bgu.ac.il on 08/01/2020.
//

#include "../include/ClientDB.h"



public:

    bool ClientDB::getIsActive() const {
        return isActive;
    }

    void ClientDB::setIsActive(bool isActive) {
        ClientDB::isActive = isActive;
    }

    const std::vector<std::string> &ClientDB::getGetBooksOfTopic() const {
        return getBooksOfTopic;
    }

    void ClientDB::setGetBooksOfTopic(const std::vector<std::string> &getBooksOfTopic) {
        ClientDB::getBooksOfTopic = getBooksOfTopic;
    }

    const std::string &ClientDB::getMyName() const {
        return myName;
    }

    void ClientDB::setMyName(const std::string &myName) {
        ClientDB::myName = myName;
    }

    const std::unordered_map<std::string, std::vector<std::string>> &ClientDB::getMyInventory() const {
        return myInventory;
    }

    void ClientDB::setMyInventory(const std::unordered_map<std::string, std::vector<std::string>> &myInventory) {
        ClientDB::myInventory = myInventory;
    }

    const std::unordered_map<std::string, std::string> &ClientDB::getBorrowedMap() const {
        return borrowedMap;
    }

    void ClientDB::setBorrowedMap(const std::unordered_map<std::string, std::string> &borrowedMap) {
        ClientDB::borrowedMap = borrowedMap;
    }

    void ClientDB::initialize(std::string &name) {

    }

const std::unordered_map<int, std::vector<std::string>> &ClientDB::getReceiptMap() const {
    return receiptMap;
}

void ClientDB::add_book_to_Inv(std::string book,std::string topic) {
    std::vector<std::string> books = myInventory.at(topic);
    bool found=false;
    for(std::string b : books){
        if(book.compare(b)==0){
            found=true;
            break;
        }
    }
    if(!found)
        books.push_back(book); //insert book only if not there already

}





