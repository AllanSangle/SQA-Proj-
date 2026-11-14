#pragma once
#include "UserAccountFiles.h"
#include "UserAccounts.h"
#include <ostream>

// ListAccounts header class
class ListAccounts {
public:
    bool hasPermission();
    void display(const UserAccountsFile&, std::ostream& out);
};