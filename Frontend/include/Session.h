#pragma once 
#include "UserAccounts.h"

class Session {
public:
    void start(UserAccounts User);
    void end();
    UserAccounts getCurrentUser();
    bool isActive();
    void addSessionCredit(Credit amount); 

private:
    UserAccounts currentUser();
    bool active();
    int64_t sessionCreditAdded();
};