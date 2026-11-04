#pragma once
#include "UserAccounts.h"
//sets buy standard users permissions
class BuyStandardUser : public UserAccounts {
public:
    using UserAccounts::UserAccounts;
    bool canBuy() const;
    bool canSell() const;
    bool isAdmin() const;
    std::string getUserType() const;
};
