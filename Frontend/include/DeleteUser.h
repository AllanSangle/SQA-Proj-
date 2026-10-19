#pragma once
#include <cstdint>
#include "Session.h"
#include "UserAccountFiles.h"
#include "AvailableGamesFile.h"
#include "GameCollectionFile.h"
#include "DailyTransactionFile.h"
class DeleteUser {
private:
    std::string username;
public:
    bool isAdmin();
    bool userExists(const std::string& username);
    void removeUser(std::string username, std::string type);
    std::string logDelete(const std::string& code, const std::string& user, const std::string& type, int64_t balance);
};
