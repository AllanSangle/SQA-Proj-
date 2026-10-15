#pragma once
#include <string>
#include <vector>

#include "Credit.h"
#include "OwnedGame.h"

struct accounts {
    std::string username;
    Credit credit;
};


class UserAccounts {
private:
    std::string username;
    std::string password;
    Credit credit;
    std::vector<OwnedGame> ownedGames;

public:
    void login(std::string username, Credit credit); 
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
    Credit getBalance();
    std::vector<OwnedGame> getOwnedGames(); 
    void addCredit(Credit credit);
};
