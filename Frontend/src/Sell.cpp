#include "Runtime.inc"

// sets the permissions for selling accounts
bool Sell::hasPermission() {
    return runtime::active && runtime::account(runtime::current).type != "BS";
}

// checks if a game with title of the game being sold already exists
bool Sell::gameAlreadyExists(const std::string& title) {
    return runtime::games.count(title) != 0;
}

// Formats the sale listing transaction using the required field widths.
std::string Sell::logSell(const std::string& code, const std::string& title, const std::string& user, int64_t value) {
    return runtime::field(code, 2) + " " + runtime::field(title, 25) + " " + runtime::field(user, 15) + " " + runtime::amountField(value, 6);
}

// executes the sell transaction
// Marks new games so the buy transaction can block them until the next session.
void Sell::execute(const std::string& title, int64_t value) {
    require(hasPermission(), "You do not have permissions to sell."); // User must be a type that has permission to sell
    require(!gameAlreadyExists(title), "Failed to publish game. Game name cannot be the same as an existing game"); // Game being sold must have a unique title
    
    // Limits game titles to 1 through 25 characters.
    require(!title.empty() && title.size() <= 25, "Failed to publish game. Name of game cannot exceed 25 characters");
    // Allows prices from $0.00 through $999.99, stored as cents.
    require(value >= 0 && value <= MAX_PRICE, "Failed to publish game. Cost of game cannot exceed 999.99");
    // Creates the validated game before adding it to the in-memory catalog.
   
    Game g(title, runtime::current, value);
    game = title;
    seller = runtime::current; // current user is the seller
    price = value;
    runtime::games.emplace(title, g); // adds the game to memory
    runtime::newGames.insert(title); // adds game to memory as newly added that session
    runtime::records.push_back(logSell("03", title, seller, price)); // logs the transaction
}