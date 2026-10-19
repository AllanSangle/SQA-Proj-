#pragma once
#include <cstdint> 
#include "UserAccounts.h"

class Session {
public:
    void start(UserAccounts User);
    void end();
    UserAccounts getCurrentUser();
    bool isActive();
    void addSessionCredit(int64_t amount); 

private:
    UserAccounts currentUser();
    bool active();
    int64_t sessionCreditAdded();
};