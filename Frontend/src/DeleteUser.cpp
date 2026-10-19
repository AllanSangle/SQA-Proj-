#include "Runtime.inc"

bool DeleteUser::isAdmin() {
    return runtime::admin();
}

// checks if the user being deleted exists
bool DeleteUser::userExists(const std::string& name) {
    return runtime::accounts.count(name) != 0;
}

// removes user
// still need to test if deleted sellers games are deleted
void DeleteUser::removeUser(std::string name, std::string type) {
    runtime::administrator();
    require(name != runtime::current, "Cannot delete the current user.");
    auto a = runtime::account(name);
    require(type == a.type, "Account type mismatch.");
    username = name;
    runtime::records.push_back(logDelete("02", name, a.type, a.balance));
    runtime::accounts.erase(name);
    for (auto i = runtime::games.begin(); i != runtime::games.end();) {
        ++i;
    }
    // removes user in memory without changing files
    runtime::owned.erase(std::remove_if(runtime::owned.begin(), runtime::owned.end(), [&](const OwnedGame& o) {
        return o.username == name;
    }),
    runtime::owned.end());
}

// logs delete transaction
std::string DeleteUser::logDelete(const std::string& code, const std::string& user, const std::string& type, int64_t balance) {
    return runtime::accountRecord(code, user, type, balance);
}
