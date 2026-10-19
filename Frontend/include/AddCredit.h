#pragma once
#include <cstdint>
#include "Session.h"
#include "UserAccountFiles.h"
#include "DailyTransactionFile.h"

class AddCredit{
    private:
    int64_t amount;
    std::string username;

    public:
    bool isAdmin(){
        return true;
    };

    void updateCredit(std::string username, int64_t amount);
    void logAddCredit(std::string code, std::string username, std::string type, int64_t amount);
};