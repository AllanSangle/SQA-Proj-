#include "Runtime.inc"

// sets the permissions for selling accounts
bool Sell::hasPermission() {
    return runtime::active && runtime::account(runtime::current).type != "BS";
}

// checks if a game with title of the game being sold already exists
bool Sell::gameAlreadyExists(const std::string& title) {
    return runtime::games.count(title) != 0;
}

// temp: logs sell transactions for testing
std::string Sell::logSell(const std::string& code, const std::string& title, const std::string& user, int64_t value) {
    return runtime::field(code, 2) + " " + runtime::field(title, 25) + " " + runtime::field(user, 15) + " " + runtime::amountField(value, 6);
}

// executes the sell transaction
// Still needs testing: newly added games not being purchaseable in the same session its published
void Sell::execute(const std::string& title, int64_t value) {
    require(hasPermission(), "You do not have permissions to sell."); // User must be a type that has permission to sell
    require(!gameAlreadyExists(title), "Game already exists."); // Game being sold must have a unique title
    Game g(title, runtime::current, value);
    game = title;
    seller = runtime::current; // current user is the seller
    price = value;
    runtime::games.emplace(title, g); // adds the game to memory
    runtime::newGames.insert(title); // adds game to memory as newly added that session
    runtime::records.push_back(logSell("03", title, seller, price)); // logs the transaction
}

