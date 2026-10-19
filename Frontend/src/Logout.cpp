#include "Runtime.inc"

// handles logging out
void Logout::logout() {
    runtime::loggedIn();
    auto& a = runtime::account(runtime::current);
    runtime::records.push_back(logSessionEnd("00", runtime::current, a.type, a.balance));
    endSession();
}

// ends session on logout
void Logout::endSession() {
    Session s;
    s.end();
}

// logs the end of session transaction
std::string Logout::logSessionEnd(const std::string& code, const std::string& user, const std::string& type, int64_t balance) {
    return runtime::accountRecord(code, user, type, balance);
}
