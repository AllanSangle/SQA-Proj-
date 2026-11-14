#include "Runtime.inc"

// starts a new session
//system to keep track of the local variables created by user 
void Session::start(UserAccounts user) {
    require(!runtime::active, "Already logged in.");
    runtime::account(user.getUsername());
    runtime::current = user.getUsername();
    runtime::active = true;
    runtime::added.clear();
    runtime::newGames.clear();
    runtime::records.clear();
}

// ends the current session
void Session::end() {
    runtime::active = false;
    runtime::current.clear();
    runtime::added.clear();
    runtime::newGames.clear();
}

// gets the current user
UserAccounts Session::getCurrentUser() {
    runtime::loggedIn();
    UserAccounts u;
    u.login(runtime::current, runtime::account(runtime::current).balance);
    return u;
}

// checks if the session is active
bool Session::isActive() {
    return runtime::active;
}

// adds session credit and checks if it exceeds the limit
void Session::addSessionCredit(int64_t amount) {
    runtime::loggedIn();
    auto& n = runtime::added[runtime::current];
    require(amount >= 0 && amount <= Max_SESSION_CREDIT - n, "Session credit limit exceeded.");
    n += amount;
}

// gets the current user
UserAccounts Session::currentUser() {
    return getCurrentUser();
}

// checks if the session is active
bool Session::active() {
    return isActive();
}

// gets the amount of session credit added to log the transaction
int64_t Session::sessionCreditAdded() {
    return runtime::active ? runtime::added[runtime::current] : 0;
}
