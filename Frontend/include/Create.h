#pragma once
#include "Session.h"
#include "UserAccountFiles.h"
#include "AvailableGamesFile.h"
#include "CurrentGamesFile.h"
#include "DailyTransactionFile.h"
class Create {
private:
    std::string username;
    UserAccounts type;
public:
    void readFile(const std::string& path);
    void writeFile(const std::string& path) const;
    bool isAdmin();
    bool userAlreadyExists(const std::string& username);
    void addUser(std::string username, std::string type, Credit credit);
    std::string logCreate(const std::string& code, const std::string& user, const std::string& type, Credit balance);
};
