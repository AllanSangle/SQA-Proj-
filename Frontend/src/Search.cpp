#include "Runtime.inc"

// Checks if a name contains the search result in it
bool searchInName(const std::string& name, const std::string& query) {
    return name.find(query) != std::string::npos;
}

void Search::display(const std::string& query, std::ostream& out) {
    runtime::loggedIn(); // you can only search if logged in
    require(!query.empty(), "Search cannot be empty."); // Makes sure that something was typed into the search
    bool gamesFound = false; // checks if there were games found or the search
    bool usersFound = false; // checks if there were users found or the search

    // displays the games with names that statisfy the search.
    out << "Games Found:\n";
    for (const auto& item : runtime::games) { // loops through games
        const auto& game = item.second;

        // displays name, seller, and price of games that statisfy the search
        if (searchInName(game.getName(), query)) {
            out << runtime::field(game.getName(), 26) << " " << runtime::field(game.getSeller(), 15) << " " << runtime::amountField(game.getPrice(), 6) << '\n';
            gamesFound = true; // results were found
        }
    }
    // tells user if there weren't any games found to statisfy their search
    if (!gamesFound) {
        out << "No games found.\n";
    }


    // displays the users with names that statisfy the search.
    out << "Users Found:\n";
    for (const auto& item : runtime::accounts) { // loops through accounts
        const auto& username = item.first;
        const auto& account = item.second;

        // checks if users names don't statisfy the search isn't in the search
        if (!searchInName(username, query)) {
            continue;
        }

        // checks if searched users are an admin
        bool isAdmin = account.type == "AA";

        // checks if admin users that statisfy the search are currently selling a game
        bool isSelling = std::any_of(
            runtime::games.begin(),
            runtime::games.end(),
            [&](const auto& game) {
                return game.second.getSeller() == username;
            }
        );

        // checks if the current user is an admin, and if an admin is being searched, and if the admin being searched isn't selling a game
        if (!runtime::admin() && isAdmin && !isSelling) {
            continue;
        }

        // displays found users
        out << runtime::field(username, 15) << " " << account.type << " " << runtime::amountField(account.balance, 9) << '\n';
        usersFound = true; // users were found
    }
    // tells user if no users satisfied their search
    if (!usersFound) { 
        out << "No users found.\n";
    }
}