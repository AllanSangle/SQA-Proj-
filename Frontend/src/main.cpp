/*
    Game Distribution System

    Inputs:
        current_user_accounts.txt
        available_games.txt
        game_collection.txt

    How to Run:
    (Requires CMAKE to be installed)
    In the Frontend folder (\Frontend) open run.bat. Once the build is complete begin entering transactions (login, logout, create, delete refund, sell, buy, addcredit, listaccounts)

    begin by entering login

    Here are the available usernames already created and can be logged into:
        AdminUser1
        FullUser1
        RegularUser1
        RegularUser2
        PublishingUser1
        PublishingUser2

*/

#include "Runtime.inc"
#include <filesystem>

namespace {
// Displays a game with fixed spacing for the title, username, and price.
void showGame(const std::string& title, const std::string& user, int64_t value) {
    std::cout << runtime::field(title, 26) << " " << runtime::field(user, 15)
              << " " << runtime::amountField(value, 6) << '\n';
}
// Checks the new username before asking for the account type or initial credit.
void checkCreateName(const std::string& name) {
    require(!name.empty() && name.size() <= 15, "Failed to create user. Usernames can't exceed 15 characters.");
    validateName(name, 15);
    require(!runtime::accounts.count(name), "Failed to create account. Account already exists with that username.");
}
// Checks the game title before asking for its price so invalid titles do not consume the next command.
void checkSellTitle(const std::string& title) {
    require(!title.empty() && title.size() <= 25, "Failed to publish game. Name of game cannot exceed 25 characters");
    validateName(title, 25);
    require(!runtime::games.count(title), "Failed to publish game. Game name cannot be the same as an existing game");
}
}

int main(int argc, char** argv) {
    try {
        // Makes sure the three input filenames and transaction output filename were provided.
        if (argc != 5) {
            std::cerr << "Usage: frontend accounts.txt games.txt collection.txt transactions.atf\n";
            return 1;
        }
        // Sets the directory containing the stored account, catalog, and ownership files.
        const std::filesystem::path data = "../stored_data";
        // Uses the fourth argument as the daily transaction output file.
        const std::filesystem::path output = argv[4];
        // Creates the output folder when a folder was included in the filename.
        if (!output.parent_path().empty())
            std::filesystem::create_directories(output.parent_path());
        // Loads the accounts that users can log into.
        Login login;
        login.readFile((data / argv[1]).string());
        // Loads the games available for purchase.
        AvailableGamesFile available;
        available.readFile((data / argv[2]).string());
        // Loads the games already owned by each user.
        GameCollectionFile collection;
        collection.readFile((data / argv[3]).string());
        // Loads the accounts used by the listaccounts transaction.
        UserAccountsFile userAccounts;
        userAccounts.readFile((data / argv[1]).string());

        // Saves the current session transactions and then logs the user out
        auto finish = [&]() {
            // Gets the current account type and balance for the end session record
            auto a = runtime::account(runtime::current);
            auto log = runtime::records;
            // Adds transaction code 00 to mark the end of the session.
            log.push_back(runtime::accountRecord("00", runtime::current, a.type, a.balance));
            // Keeps transactions from earlier sessions when writing the new session records.
            auto allRecords = std::filesystem::exists(output) ? readRecords(output.string(), 0) : std::vector<std::string>{};
            allRecords.insert(allRecords.end(), log.begin(), log.end());
            writeRecords(output.string(), allRecords, 0);
            // Ends the session only after writing succeeds, so a failed save can be retried
            Session s;
            s.end();
            std::cout << "Successfully Logged Out!\n";
        };

        // Stores the transaction entered by the user.
        std::string command;
        // Continues asking for transactions until the input ends.
        while (std::cout << "Enter a transaction:\n" && std::getline(std::cin, command)) {
            try {
                // Removes surrounding whitespace from the transaction name.
                command = runtime::trim(command);
                // Asks again if the user enters a blank line.
                if (command.empty()) continue;
                // Starts a session for an existing user and displays the available games
                if (command == "login") {
                    login.login();
                    std::cout << "Successfully Logged In. Welcome, " << runtime::current << '\n';
                    GameCatalogPage c;
                    c.display(available, std::cout);
                    continue;
                }
                // Checks that a session exists before saving transactions and logging out
                if (command == "logout") {
                    require(runtime::active, "You can't logout without being logged in");
                    finish();
                    continue;
                }
                // Requires an active session for all remaining transactions
                runtime::loggedIn();
                auto role = runtime::account(runtime::current).type;
                // Allows an admin to enter the details for a new account.
                if (command == "create") {
                    require(runtime::admin(), "Failed to create account. " + role + " users cannot create accounts. You must be an admin to create account");
                    auto name = runtime::ask("Enter the new account's username:");
                    // Rejects invalid or duplicate usernames before reading more account details.
                    checkCreateName(name);
                    auto type = runtime::ask("Enter the new account's type:");
                    require(type == "AA" || type == "FS" || type == "BS" || type == "SS", "Failed to create user. Not a correct account type");
                    // Reads the initial credit after confirming that the account type is valid.
                    auto value = runtime::money(runtime::ask("Initial credit:"));
                    // Creates the account in memory and records the creation transaction.
                    Create c;
                    c.addUser(name, type, value);
                    std::cout << "Successfully created " << type << " user " << name << ".\n";
                } else if (command == "delete") {
                    // Only admins can delete accounts.
                    require(runtime::admin(), "You can't delete accounts as a " + role + " user. You must be an admin to delete accounts.");
                    auto name = runtime::ask("Enter the username of the account to delete:");
                    require(runtime::accounts.count(name), "Failed to delete user. Account with that username does not exist.");
                    // Removes the account and its related game data, then records the deletion.
                    DeleteUser d;
                    d.removeUser(name, runtime::account(name).type);
                    std::cout << "Successfully deleted " << name << ".\n";
                } else if (command == "sell") {
                    // Checks selling permission before asking for game details.
                    Sell s;
                    require(s.hasPermission(), "Failed to sell game. You don't have permissions to sell games.");
                    auto title = runtime::ask("Enter the name of the game to sell:");
                    // Rejects invalid or existing game titles before asking for a price.
                    checkSellTitle(title);
                    auto value = runtime::money(runtime::ask("Enter the cost of the game to sell:"));
                    // Publishes the game in memory and records the sale listing
                    s.execute(title, value);
                    std::cout << "Successfully published:\n";
                    showGame(title, runtime::current, value);
                } else if (command == "buy") {
                    // Checks buying permission before asking which game to purchase.
                    Buy b;
                    require(b.hasPermission(), "Failed to purchase game. You're account type doesn't have permission to purchase games.");
                    auto title = runtime::ask("Enter the name of the game to buy:");
                    // Checks that the game exists before asking for the seller
                    require(runtime::games.count(title), "Failed to buy game. Game with that name does not exist");
                    // Prevents buying a game published during the current session
                    require(!runtime::newGames.count(title), "Failed to buy game. Start a new session to purchase new games.");
                    auto seller = runtime::ask("Enter the name of the game's seller:");
                    // Adds ownership, transfers the price between accounts, and records the purchas
                    b.execute(title, seller);
                    std::cout << "Successfully purchased:\n";
                    showGame(title, seller, b.getPrice());
                } else if (command == "refund") {
                    // Only admins can transfer a refund from a seller to a buyer.
                    require(runtime::admin(), "Failed to refund. Your account type doesn't have permission to refund.");
                    auto buyer = runtime::ask("Enter the name of the user to refund:");
                    require(runtime::accounts.count(buyer), "Failed to refund. Account with that username does not exist.");
                    // Rejects buyers with no games before reading the seller and refund amount.
                    require(std::any_of(runtime::owned.begin(), runtime::owned.end(), [&](const OwnedGame& o) { return o.username == buyer; }), "Failed to refund. This user doesn't own any games.");
                    auto seller = runtime::ask("Enter the name of the seller of the game being refunded:");
                    auto value = runtime::money(runtime::ask("Enter the amount to refund:"));
                    Refund r;
                    // Validates the refund, updates both balances, and records the transaction
                    r.execute(buyer, seller, value);
                    std::cout << "Successfully refunded:\n";
                    // Display an owned game from this seller; the refund record
                    // itself identifies the two accounts and the transfer amount
                    auto owned = std::find_if(runtime::owned.begin(), runtime::owned.end(), [&](const OwnedGame& o) {
                        auto g = runtime::games.find(o.game);
                        return o.username == buyer && g != runtime::games.end() && g->second.getSeller() == seller;
                    });
                    showGame(owned->game, buyer, value);
                } else if (command == "addcredit") {
                    // Reads the credit amount first, matching the transaction input order
                    auto value = runtime::money(runtime::ask("Enter the amount of credits to add:"));
                    auto user = runtime::admin() ? runtime::ask("Enter the username of the user to add credits to:") : runtime::current;
                    AddCredit a;
                    // Checks the session and balance limits before adding credit and recording it
                    a.updateCredit(user, value);
                    std::cout << "Successfully added $" << runtime::moneyText(value);
                    if (runtime::admin()) std::cout << " to " << user;
                    std::cout << ".\n";
                } else if (command == "listaccounts") {
                    runtime::administrator();
                    // Displays the active accounts after checking administrator permission
                    ListAccounts l;
                    l.display(userAccounts, std::cout);
                } else {
                    // Reports input that is not a supported transaction.
                    throw std::runtime_error("Unknown command");
                }
            } catch (const std::exception& e) {
                // Displays a transaction error and returns to the command prompt.
                std::cout << "ERROR: " << e.what() << '\n';
            }
        }
        // Saves an active session if the input ends before an explicit logout.
        if (runtime::active) finish();
        return 0;
    } catch (const std::exception& e) {
        // Reports startup or final save errors and returns a failure exit code.
        std::cerr << "ERROR: " << e.what() << '\n';
        return 1;
    }
}
