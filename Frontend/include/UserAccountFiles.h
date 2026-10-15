#pragma once
#include <vector>
#include "UserAccounts.h"
class UserAccountsFile {
private:
    std::vector<UserAccounts> Users;
public:
    void readFile(const std::string& path);
    void writeFile(const std::string& path) const;
    std::vector<UserAccounts> findUser(const std::string& username) const;
    bool userExists(const std::string& username) const; 
};

