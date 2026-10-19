#include "Runtime.inc"

void Session::start(UserAccounts user) {
    require(!runtime::active, "Already logged in.");
    runtime::account(user.getUsername());
    runtime::current = user.getUsername();
    runtime::active = true;
    runtime::added.clear();
    runtime::newGames.clear();
    runtime::records.clear();
}

void Session::end() {
    runtime::active = false;
    runtime::current.clear();
    runtime::added.clear();
    runtime::newGames.clear();
}

UserAccounts Session::getCurrentUser() {
    runtime::loggedIn();
    UserAccounts u;
    u.login(runtime::current, runtime::account(runtime::current).balance);
    return u;
}

bool Session::isActive() {
    return runtime::active;
}

void Session::addSessionCredit(int64_t amount) {
    runtime::loggedIn();
    auto& n = runtime::added[runtime::current];
    require(amount >= 0 && amount <= Max_SESSION_CREDIT - n, "Session credit limit exceeded.");
    n += amount;
}

UserAccounts Session::currentUser() {
    return getCurrentUser();
}

bool Session::active() {
    return isActive();
}

int64_t Session::sessionCreditAdded() {
    return runtime::active ? runtime::added[runtime::current] : 0;
}
