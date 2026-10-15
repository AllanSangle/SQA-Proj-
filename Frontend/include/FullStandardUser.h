#pragma once
#include "UserAccounts.h"
class FullStandardUser : public UserAccounts {
public:
    using UserAccounts::UserAccounts;
    bool canBuy() const;
    bool canSell() const;
    bool isAdmin() const;
    std::string getUserType() const;
};
