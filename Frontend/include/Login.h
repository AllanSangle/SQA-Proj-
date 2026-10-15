#pragma once
#include "Session.h"
#include "UserAccountFiles.h"

class Login {
private:
    void username();
    std::vector<UserAccounts> users(); 
public:
    bool isLoggedIn(){return false;};
    void login();
    void readFile(const std::string& path);
    void writeFile(const std::string& path) const;
    void startSession(std::string username);
};