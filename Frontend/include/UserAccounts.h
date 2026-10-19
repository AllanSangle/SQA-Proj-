#pragma once
#include <string>
#include <vector>

#include <cstdint>
#include <string>
#include "OwnedGame.h"

struct accounts {
    std::string username;
    int64_t credit;
};


class UserAccounts {
private:
    std::string username;
    std::string password;
    int64_t credit;
    std::vector<OwnedGame> ownedGames;

public:
    void login(std::string username, int64_t credit); 
    void logout(); 
    bool isLoggedIn();
    bool canBuy(); 
    bool canSell(); 
    bool isAdmin(); 
    bool canAddCredit(); 
    bool canCreate(); 
    bool canRefund(); 
    std::string getUserType(); 
    std::string getUsername(); 
    int64_t getBalance();
    std::vector<OwnedGame> getOwnedGames(); 
    void addCredit(int64_t credit);
};
