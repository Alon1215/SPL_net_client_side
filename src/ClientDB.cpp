//
// Created by michalon@wincs.cs.bgu.ac.il on 08/01/2020.
//

#include "../include/ClientDB.h"




ClientDB::ClientDB(): myTopics(), wishList(), myInventory(), borrowedMap(), receiptMap(),
                      isActive(false), isShouldTerminate(false), myName(), receiptNumCounter(0),
                      subscriptionId(0), wantLogout(false),
                      wish_lock(), inv_lock(),borrow_lock(),receipt_lock(),topic_lock(),cv(){

}
    bool ClientDB::getIsActive() const {
        return isActive;
    }

    void ClientDB::setIsActive(bool isActive) {
        ClientDB::isActive = isActive;
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

 std::unordered_map<int, std::vector<std::string>> ClientDB::getReceiptMap()  {
    return receiptMap;
}

//------ methods which use lockers:

void ClientDB::add_book_to_Inv(std::string book,std::string topic) {
    std::lock_guard<std::mutex> lock(inv_lock); //lock sending

    std::vector<std::string> &books = myInventory.at(topic);

    if(myInventory.count(topic)==0) {
        myInventory.insert(std::make_pair(topic,std::vector<std::string>()));
    }
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
    std::lock_guard<std::mutex> lock(inv_lock); //lock sending

    std::vector<std::string> &books = myInventory.at(topic);
    for(int i=0;i< (signed) books.size();i++){
        if(book==books.at(i)){
            books.erase(books.begin()+i);
            return true;
        }
    }
    return false;
}
std::vector<std::string> ClientDB::get_topic_books(std::string topic) {
    std::lock_guard<std::mutex> lock(inv_lock); //lock
    return myInventory.at(topic);
}

bool ClientDB::remove_book_from_wishList(std::string book) {;
    std::lock_guard<std::mutex> lock(wish_lock); //lock sending


    for(int i=0;i< (signed) wishList.size();i++){
        if(book==wishList.at(i)){
            wishList.erase(wishList.begin()+i);
            return true;
        }
    }
    return false;
}

void ClientDB::add_book_to_wishList(std::string book) {
    std::lock_guard<std::mutex> lock(wish_lock); //lock sending

    wishList.push_back(book);
}
bool ClientDB::wishList_contains(const std::string& book) {
    std::lock_guard<std::mutex> lock(wish_lock); //lock
    for(std::string b: wishList){
        if(b==book)
            return true;
    }
    return false;
}



bool ClientDB::remove_from_borrowdMap(std::string book) {
    std::lock_guard<std::mutex> lock(borrow_lock); //lock sending

    if(borrowedMap.count(book)==0)
        return false;
    else{
        borrowedMap.erase(book);
        return true;
    }
}

void ClientDB::add_book_to_borrowdMap(std::string book ,std::string loaner_name ) {
    std::lock_guard<std::mutex> lock(borrow_lock); //lock sending

    borrowedMap.insert(std::make_pair(book, loaner_name));
    //add to map
}
std::string ClientDB::get_loaner_name(std::string book) {
    std::lock_guard<std::mutex> lock(borrow_lock); //lock
    if(borrowedMap.count(book)==1)
        return borrowedMap.at(book);
    else return "";
}


void ClientDB::add_receipt(int receiptID, std::vector<std::string> reciptInfo) {
    std::lock_guard<std::mutex> lock(receipt_lock); //lock sending

    receiptMap.insert(std::make_pair(receiptID, reciptInfo));

}

std::vector<std::string> &ClientDB::get_receipt_info(int receiptID) {
    std::lock_guard<std::mutex> lock(receipt_lock); //lock sending
    return  receiptMap.at(receiptID);
}

void ClientDB::remove_from_myTopics(std::string topic) {
    std::unique_lock<std::mutex> lck(topic_lock);
    myTopics.erase(topic);
    if(wantLogout && myTopics.empty())
        cv.notify_all();
}

void ClientDB::add_to_myTopics(std::string topic , int subID) {
    std::lock_guard<std::mutex> lock(topic_lock); //lock

    myTopics.insert(std::make_pair(topic, subID));
}

int ClientDB::get_subscription_id(std::string topic) {
    std::lock_guard<std::mutex> lock(topic_lock); //lock

    return myTopics.at(topic);
}

bool ClientDB::inv_contains_book(std::string book, std::string topic)  { //returns true if client has book in Inv
    std::lock_guard<std::mutex> lock(inv_lock); //lock

    if (myInventory.count(topic) == 1) {
        std::vector<std::string> &books = myInventory.at(topic);
        for (std::string b: books)
            if (book == b) {
                return true;
            }
    }
    return false;
}

bool ClientDB::is_inv_contains_topic(std::string topic) {
    std::lock_guard<std::mutex> lock(inv_lock); //lock
    return myInventory.count(topic) == 1;
}

void ClientDB::add_topic_to_inv(std::string topic) {
    std::lock_guard<std::mutex> lock(inv_lock); //lock sending
    myInventory.insert(std::make_pair(topic, std::vector<std::string>()));
}


// -----------------

void ClientDB::setIsShouldTerminate(bool isShouldTerminate) {//set should terminate
    ClientDB::isShouldTerminate = isShouldTerminate;
}

bool ClientDB::getIsShouldTerminate1() const {
    return isShouldTerminate;
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

std::unordered_map<std::string, int > ClientDB::getMyTopics()  {
    return myTopics;
}

std::vector<std::string> &ClientDB::getWishList() {
    return wishList;
}

bool ClientDB::isWantLogout() const {
    return wantLogout;
}

void ClientDB::setWantLogout(bool wantLogout) {
    ClientDB::wantLogout = wantLogout;
}

 std::mutex &ClientDB::getTopicLock()  {
    return topic_lock;
}

 std::condition_variable &ClientDB::getCv()  {
    return cv;
}





















