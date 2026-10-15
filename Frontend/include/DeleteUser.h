#pragma once
#include "Session.h"
#include "UserAccountFiles.h"
#include "AvailableGamesFile.h"
#include "CurrentGamesFile.h"
#include "DailyTransactionFile.h"
class DeleteUser {
private:
    std::string username;
public:
    void readFile(const std::string& path);
    void writeFile(const std::string& path) const;
    bool isAdmin();
    bool userExists(const std::string& username);
    void removeUser(std::string username, std::string type);
    std::string logDelete(const std::string& code, const std::string& user, const std::string& type, Credit balance);
};
