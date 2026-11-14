#pragma once
#include <cstdint> 
#include "UserAccounts.h"

// Session header class
//system to keep track of the local variables created by user
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