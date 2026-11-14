#pragma once
#include "UserAccounts.h"

//sets sell standard users permissions
class SellStandardUser : public UserAccounts {
public:
    using UserAccounts::UserAccounts;
    bool canBuy() const;
    bool canSell() const;
    bool isAdmin() const;
    std::string getUserType() const;
};
