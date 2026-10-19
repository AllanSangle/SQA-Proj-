#include "Runtime.inc"

// Displays user account structure and permissions

// User login
void UserAccounts::login(std::string name, int64_t balance) {
    validateName(name, 15); // Users have a max username length
    require(balance >= 0 && balance <= MAX_CREDIT, "Invalid balance."); // User's have a max credit balance
    username = std::move(name);
    credit = balance;
}

// users can logout if current;y logged in
void UserAccounts::logout() {
    if (runtime::current == username) {
        runtime::active = false;
        runtime::current.clear();
    }
}

// 
bool UserAccounts::isLoggedIn() {
    return runtime::active && runtime::current == username;
}

std::string UserAccounts::getUserType() {
    auto i = runtime::accounts.find(username);
    return i == runtime::accounts.end() ? "FS" : i->second.type;
}

// User transaction permission based on their user type
bool UserAccounts::canBuy() {
    return getUserType() != "SS";
}

bool UserAccounts::canSell() {
    return getUserType() != "BS";
}

bool UserAccounts::isAdmin() {
    return getUserType() == "AA";
}

bool UserAccounts::canAddCredit() {
    return true;
}

bool UserAccounts::canCreate() {
    return isAdmin();
}

bool UserAccounts::canRefund() {
    return isAdmin();
}

// gets a user's username
std::string UserAccounts::getUsername() {
    return username;
}

// gets the users balance
int64_t UserAccounts::getBalance() {
    auto i = runtime::accounts.find(username);
    return i == runtime::accounts.end() ? (username.empty() ? 0 : credit) : i->second.balance;
}

// gets users owned games
std::vector<OwnedGame> UserAccounts::getOwnedGames() {
    ownedGames.clear();
    for (const auto& o : runtime::owned)
        if (o.username == username)
            ownedGames.push_back(o);
    return ownedGames;
}

// users can add credit
void UserAccounts::addCredit(int64_t amount) {
    int64_t old = getBalance();
    require(amount >= -old && amount <= MAX_CREDIT - old, "Balance out of range.");
    credit = old + amount;
    auto i = runtime::accounts.find(username);
    if (i != runtime::accounts.end())
        i->second.balance = credit;
}
