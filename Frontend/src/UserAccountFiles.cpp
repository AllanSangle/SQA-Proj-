#include "Runtime.inc"

// For reading user accounts file
void UserAccountsFile::readFile(const std::string& path) {
    runtime::loadAccounts(path);
    Users.clear();
    for (const auto& a : runtime::accounts) {
        UserAccounts u;
        u.login(a.first, a.second.balance);
        Users.push_back(u);
    }
}

void UserAccountsFile::writeFile(const std::string& path) const {
    runtime::saveLog(path);
}

// For searching for user in user accounts file
std::vector<UserAccounts> UserAccountsFile::findUser(const std::string& name) const {
    auto i = runtime::accounts.find(name);
    if (i == runtime::accounts.end())
        return {};
    UserAccounts u;
    u.login(name, i->second.balance);
    return {u};
}

// Check if users exists in the file
bool UserAccountsFile::userExists(const std::string& name) const {
    return runtime::accounts.count(name) != 0;
}
