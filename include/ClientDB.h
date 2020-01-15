//
// Created by michalon@wincs.cs.bgu.ac.il on 08/01/2020.
//

#ifndef SPL_NET_CLIENT_CLIENTDB_H
#define SPL_NET_CLIENT_CLIENTDB_H


#include <vector>
#include <string>
#include <unordered_map>
#include <mutex>
#include <condition_variable>

class ClientDB {

private:

    std::unordered_map<std::string, int> myTopics; // key = topic name , val = sub id
    bool isActive;
    std::vector<std::string> wishList;
    std::unordered_map<std::string,std::vector<std::string>> myInventory; //1st = topic , 2nd = vector of books
    std::unordered_map<std::string,std::string> borrowedMap; // key = book, val = name of loaner
    std::unordered_map<int,std::vector<std::string>> receiptMap; //key=receipt num, val=vector of important info (first cell = type of msg (disconnect,subscribe,...)
    std::condition_variable cv;
private:
    // second= topic (if needed),third=subscription iD
    bool isShouldTerminate;
    std::string myName;
public:
    bool isWantLogout() const;
    std::condition_variable &getCv() ;
    void setWantLogout(bool wantLogout);

private:
    int receiptNumCounter; //this wil be a unique number of each receipt
    int subscriptionId;
    std::mutex wish_lock;
    std::mutex inv_lock;
    std::mutex borrow_lock;
    std::mutex receipt_lock;
    std::mutex topic_lock;
    bool wantLogout;


public:
    bool remove_book_from_Inv(std::string book, std::string topic);

    std::unordered_map<int, std::vector<std::string>> getReceiptMap();

    void setIsActive(bool isActive);

    std::vector<std::string> getBooksOfTopic;

    bool getIsActive() const;

    const std::vector<std::string> &getGetBooksOfTopic() const;

    void setGetBooksOfTopic(const std::vector<std::string> &getBooksOfTopic);

    const std::string &getMyName() const;

    void setMyName(const std::string &myName);

    const std::unordered_map<std::string, std::vector<std::string>> &getMyInventory() const;

    void setMyInventory(const std::unordered_map<std::string, std::vector<std::string>> &myInventory);

     std::unordered_map<std::string, std::string> &getBorrowedMap() ;

    void setBorrowedMap(const std::unordered_map<std::string, std::string> &borrowedMap);

    void add_book_to_Inv(std::string book,std::string topic);

    bool inv_contains_book(std::string book, std::string topic);

    std::vector<std::string> &getWishList();

    bool remove_book_from_wishList(std::string book);

    void add_book_to_wishList(std::string book);

    bool remove_from_borrowdMap(std::string book);

    void add_book_to_borrowdMap(std::string book,std::string loaner_name);

    void add_receipt(int receiptID, std::vector<std::string> receiptInfo);

    std::vector<std::string> &get_receipt_info(int receiptID);

    void remove_from_myTopics(std::string topic);

    void add_to_myTopics(std::string topic, int subID);


    void setIsShouldTerminate(bool isShouldTerminate);

    bool getIsShouldTerminate1() const;

    int getSubIdAndInc();

    int getRecIdAndInc();

    std::unordered_map<std::string, int> getMyTopics();

    bool wishList_contains(const std::string &book);

    int get_subscription_id(std::string topic);

    std::vector<std::string> get_topic_books(std::string topic);

    std::string get_loaner_name(std::string book);

    bool is_inv_contains_topic(std::string topic);

    void add_topic_to_inv(std::string topic);

     std::mutex &getTopicLock() ;


//    std::string status_make(std::string topic);

    //constructor:
    ClientDB();

};


#endif //SPL_NET_CLIENT_CLIENTDB_H
