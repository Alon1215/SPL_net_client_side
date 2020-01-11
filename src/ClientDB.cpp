//
// Created by michalon@wincs.cs.bgu.ac.il on 08/01/2020.
//

#include "../include/ClientDB.h"


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

    std::unordered_map<std::string, std::string> &ClientDB::getBorrowedMap()  {
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
bool ClientDB::remove_book_from_Inv(std::string book,std::string topic) {
    std::vector<std::string> books = myInventory.at(topic);
    for(int i=0;i<books.size();i++){
        if(book==books.at(i)){
            books.erase(books.begin()+i); //TODO: hope this works
            return true;
        }
    }
    return false;
}
bool ClientDB::remove_book_from_wishList(std::string book,std::string topic) {;
    for(int i=0;i<wishList.size();i++){
        if(book==wishList.at(i)){
            wishList.erase(wishList.begin()+i); //TODO: hope this works
            return true;
        }
    }
    return false;
}

const Protocol &ClientDB::getProtocol() const {
    return protocol;
}

void ClientDB::setIsShouldTerminate(bool isShouldTerminate) {
    ClientDB::isShouldTerminate = isShouldTerminate;
}

bool ClientDB::getIsShouldTerminate1() const {
    return isShouldTerminate;
}




ClientDB::ClientDB(): isShouldTerminate(false), myName(), isActive(false), myInventory(), borrowedMap(), receiptMap(), protocol(),receiptNumCounter(0), subscriptionId(0) {
    //TODO: REMEMBER TO update due to more fields added
    //TODO: check if DB holds an instance of protocol
}

//next 2 method are returning the counter (for each one) and incrementing it:

int ClientDB::getSubIdAndInc() {
    subscriptionId ++;
    return subscriptionId - 1;
}

int ClientDB::getRecIdAndInc() {
    receiptNumCounter ++;
    return receiptNumCounter - 1;
}


bool ClientDB::inv_contains_book(std::string book, std::string topic) const { //returns true if client has book in Inv
    std::vector<std::string> books = myInventory.at(topic);
    for(std::string b: books)
        if(book==b){
            return true;
        }
    return false;
}

bool ClientDB::wishList_contains(std::string book) {
    for(std::string b: wishList){
        if(b==book)
            return true;
    }
    return false;
}

 std::vector<std::string> &ClientDB::getWishList()  {
    return wishList;
}








