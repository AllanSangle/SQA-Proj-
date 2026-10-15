#pragma once
#include "Session.h"
#include "UserAccountFiles.h"
#include "DailyTransactionFile.h"

class AddCredit{
    private:
    Credit amount;
    std::string username;

    public:
    bool isAdmin(){return true;};
    void readFile(const std::string& path);
    void writeFile(const std::string& path) const;
    void updateCredit(std::string username, int64_t amount);
    int64_t getSessionCredits();
    void logAddCredit(std::string code, std::string username, std::string type, int64_t amount);


};