#pragma once
#include "Session.h"
#include "DailyTransactionFile.h"
class Logout {
    public:
        bool isLoggedIn(){return true;};
        void logout();
        void readFile(const std::string& path);
        void writeFile(const std::string& path) const;
        void endSession();
        std::string logSessionEnd(const std::string& code, const std::string& user, const std::string& type, Credit balance);
};
