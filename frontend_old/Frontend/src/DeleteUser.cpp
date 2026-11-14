/*
    Game Distribution System - DeleteUser

    Deletes accounts and removes their related listings and ownership records.
    Account and game changes are kept in memory.
    Transaction records are written to the output file when the session ends.
*/

#include "Runtime.inc"

bool DeleteUser::isAdmin() {
    return runtime::admin();
}

// checks if the user being deleted exists
bool DeleteUser::userExists(const std::string& name) {
    return runtime::accounts.count(name) != 0;
}

// removes user
// Remove the seller's listings and associated ownership records as well
void DeleteUser::removeUser(std::string name, std::string type) {
    runtime::administrator();
    // Prevents deleting the account that is currently logged in
    require(name != runtime::current, "Failed to delete account. You can't delete an account you're currently signed into.");
    auto a = runtime::account(name);
    require(type == a.type, "Account type mismatch.");
    username = name;
    // Records the account type and balance before removing the account
    runtime::records.push_back(logDelete("02", name, a.type, a.balance));
    runtime::accounts.erase(name);

    // Finds listings belonging to the deleted seller.
    for (auto i = runtime::games.begin(); i != runtime::games.end();) {
        if (i->second.getSeller() == name) {
            const auto title = i->first;// Removes ownership references to the seller's deleted game
            runtime::owned.erase(std::remove_if(runtime::owned.begin(), runtime::owned.end(), [&](const OwnedGame& o) { return o.game == title; }), runtime::owned.end());// Removes the game from the current session's new-game tracking
            runtime::newGames.erase(title);
            // Erases the listing and continues from the next valid iterator.
            i = runtime::games.erase(i);
        } else {
            ++i;
        }
    }
    // Removes ownership records for games bought by the deleted account.
    runtime::owned.erase(std::remove_if(runtime::owned.begin(), runtime::owned.end(), [&](const OwnedGame& o) {
        return o.username == name;
    }),
    runtime::owned.end());
}

// logs delete transaction
std::string DeleteUser::logDelete(const std::string& code, const std::string& user, const std::string& type, int64_t balance) {
    return runtime::accountRecord(code, user, type, balance);
}
