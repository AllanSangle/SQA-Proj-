#include "Runtime.inc"

// sets users types with buy permissions
bool Buy::hasPermission() {
    return runtime::active && runtime::account(runtime::current).type != "SS";
}

// gets price of game
int64_t Buy::getPrice() {
    return price;
}

// checks if a user already owns a game
bool Buy::gameAlreadyOwned() {
    return runtime::owns(buyer, game);
}

// handles adding the game to a user in memory for testing without editing files
void Buy::addGame(std::string user, std::string title) {
    runtime::account(user);
    require(!runtime::owns(user, title), "Failed to buy game. You can't buy games you already own"); // Users cant buy games they already own
    runtime::owned.push_back({user, title}); // Purchased games get added in memory for testability without editing files
}


// Formats the purchase transaction using the required field widths.
std::string Buy::logBuy(const std::string& code, const std::string& title, const std::string& vendor, const std::string& user, int64_t value) {
    return runtime::field(code, 2) + " " + runtime::field(title, 25) + " " + runtime::field(vendor, 15) + " " + runtime::field(user, 15) + " " + runtime::amountField(value, 6);
}


// Handles buying
// Validate ownership, availability and both balances before changing state.

void Buy::execute(const std::string& title, const std::string& vendor) {
    require(hasPermission(), "You do not have permissions to buy."); // User must be a type that has permission to buy
    auto i = runtime::games.find(title); // searches for the game by title
    require(i != runtime::games.end(), "Failed to buy game. Game with that name does not exist"); 
    // Makes sure the entered seller matches the catalog listing.
    require(i->second.getSeller() == vendor, "Seller does not match.");
    // Blocks games published in the current session.
    require(!runtime::newGames.count(title), "Failed to buy game. Start a new session to purchase new games.");
    // Prevents a seller from buying their own listing.
    require(vendor != runtime::current, "Failed to buy game. You cannot buy your own game.");
    buyer = runtime::current; // Current user is the buyer
    seller = vendor; 
    game = title;
    price = i->second.getPrice();
    
    require(!gameAlreadyOwned(), "Failed to buy game. You can't buy games you already own"); // Users cant buy games they already own

    // gets buyer and seller account information for their balance
    auto& b = runtime::account(buyer);
    auto& s = runtime::account(seller);
    require(b.balance >= price, "Failed to purchase game. You don't have enough credits."); // Users must have enough credits to buy the game


    // Checks that receiving the payment will not put the seller over the balance limit.
    require(price <= MAX_CREDIT - s.balance, "Failed to buy game. Seller balance limit exceeded.");
    // Updates ownership and transfers the price only after all purchase checks pass.
    addGame(buyer, game);
    b.balance -= price;
    s.balance += price;
    // Adds transaction code 04 to the session log for writing at logout.
    runtime::records.push_back(logBuy("04", game, seller, buyer, price));
}
