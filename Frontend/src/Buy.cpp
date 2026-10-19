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
    require(!runtime::owns(user, title), "Game already owned."); // Users can't buy games they already own
    runtime::owned.push_back({user, title}); // Purchased games get added in memory for testability without editing files
}


// temp: logs buy transaction
std::string Buy::logBuy(const std::string& code, const std::string& title, const std::string& vendor, const std::string& user, int64_t value) {
    return runtime::field(code, 2) + " " + runtime::field(title, 25) + " " + runtime::field(vendor, 15) + " " + runtime::field(user, 15) + " " + runtime::amountField(value, 6);
}


// Handles buying

// Still needs testing:
    // - Not allowing sellers to be capable of buying to buy their own games
    // - Sellers balance exceeding the credit limit if a user buys a game from them 
    // - New games not being purchaseable in the same session

void Buy::execute(const std::string& title, const std::string& vendor) {
    require(hasPermission(), "You do not have permissions to buy."); // User must be a type that has permission to buy
    auto i = runtime::games.find(title); // searches for the game by title
    require(i != runtime::games.end(), "Game not found."); 
    require(i->second.getSeller() == vendor, "Seller does not match.");
    buyer = runtime::current; // Current user is the buyer
    seller = vendor; 
    game = title;
    price = i->second.getPrice();
    
    require(!gameAlreadyOwned(), "Game already owned."); // Users can't buy games they already own

    // gets buyer and seller account information for their balance
    auto& b = runtime::account(buyer);
    auto& s = runtime::account(seller);
    require(b.balance >= price, "Insufficient credit."); // Users must have enough credits to buy the game


    addGame(buyer, game);
    b.balance -= price;
    s.balance += price;
    runtime::records.push_back(logBuy("04", game, seller, buyer, price));
}