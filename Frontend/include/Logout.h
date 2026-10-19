#pragma once
#include <cstdint>
#include "Session.h"
#include "DailyTransactionFile.h"
class Logout {
    public:
        bool isLoggedIn(){return true;};
        void logout();
        void endSession();
        std::string logSessionEnd(const std::string& code, const std::string& user, const std::string& type, int64_t balance);
};
