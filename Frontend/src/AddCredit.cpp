#include "Runtime.inc"

// temp: logs add credit transaction
void AddCredit::logAddCredit(std::string code, std::string user, std::string type, int64_t value) {
    runtime::records.push_back(runtime::accountRecord(code, user, type, value));
}

// updates the credit for the user from the addcredit transaction
// Still neeeds testing for a max 1000 credit limit that can be added a session
void AddCredit::updateCredit(std::string user, int64_t value) {
    runtime::loggedIn();
    require(runtime::admin() || user == runtime::current, "Only an admin can add credit to another user.");

    auto& a = runtime::account(user);

    // makes user user balance can't be exceeded from the addcredit transaction
    require(value <= MAX_CREDIT - a.balance, "Account balance limit exceeded.");

    // logs added credit transaction
    amount = value;
    username = user;
    a.balance += value;
    runtime::added[user] += value;
    logAddCredit("06", user, a.type, value);
}
