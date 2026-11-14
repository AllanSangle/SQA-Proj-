#include "Runtime.inc"

// Adds the credit transaction to the current session log.
void AddCredit::logAddCredit(std::string code, std::string user, std::string type, int64_t value) {
    runtime::records.push_back(runtime::accountRecord(code, user, type, value));
}

// updates the credit for the user from the addcredit transaction
// Enforce the account session allowance before changing state
void AddCredit::updateCredit(std::string user, int64_t value) {
    runtime::loggedIn();
    // Only admins may add credit to a different account.
    require(runtime::admin() || user == runtime::current, "Only an admin can add credit to another user.");

    // Rejects missing or deleted recipient accounts.
    require(runtime::accounts.count(user), "Failed to add credits. No accounts exist with that username.");
    auto& a = runtime::account(user);
    // Limits the total credit added to this recipient to $1 000 per session
    require(value >= 0 && value <= Max_SESSION_CREDIT - runtime::added[user], "Failed to add credits. You can't add more than $1000.00 in one session.");

    // Makes sure the added credit will not exceed the account balance limit
    require(value <= MAX_CREDIT - a.balance, "Failed to give credits to user. User balance is $" + runtime::moneyText(a.balance) + " and would exceed the maximum credit limit of $999999.99.");

    // logs added credit transaction
    amount = value;
    username = user;
    a.balance += value;
    runtime::added[user] += value;
    logAddCredit("06", user, a.type, value);
}
