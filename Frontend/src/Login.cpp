#include "Runtime.inc"

bool Login::isLoggedIn() {
    return runtime::active;
}

// logging in
void Login::login() {
    require(!isLoggedIn(), "Already logged in."); // can't login if already logged in
    startSession(runtime::ask("Username: ")); // starts session when valid username is typed
}

// reads the user accounts file to see if entered username's exist
void Login::readFile(const std::string& path) {
    UserAccountsFile f;
    f.readFile(path);
    users.clear();
    for (const auto& a : runtime::accounts) {
        auto found = f.findUser(a.first);
        users.push_back(found.front());
    }
}

void Login::writeFile(const std::string& path) const {
    runtime::saveLog(path);
}

// starts a session for the logged in user
void Login::startSession(std::string name) {
    auto& a = runtime::account(name);
    UserAccounts u;
    u.login(name, a.balance);
    Session s;
    s.start(u);
    username = std::move(name);
}
