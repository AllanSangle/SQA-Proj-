#pragma once
#include "Session.h"
#include "UserAccountFiles.h"
class Login {
private:
    std::string username;
    std::vector<UserAccounts> users;
public:
    bool isLoggedIn();
    void login();
    void readFile(const std::string& path);
    void writeFile(const std::string& path) const;
    void startSession(std::string username);
};
