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

int main(int argc, char** argv) {
    try {
        // Makes sure that the required command line arguements were inputted correctly
        if (argc != 5) {
            std::cerr << "INPUT ERROR! In Frontend/build/debug Enter: ./frontend currentaccounts.txt availablegames.txt gamescollection.txt transout.atf\n";
            return 1;
        }

        // Sets the path for the directory with the input files (contains user and game information).
        std::filesystem::path data = "../stored_data";

        // Sets the directory for the inputed filename in the command.
        std::filesystem::path accountsFile = data / argv[1];
        std::filesystem::path availableFile = data / argv[2];
        std::filesystem::path collectionFile = data / argv[3];

        // The fourth argument is the daily transaction file.
        std::filesystem::path output = argv[4];

        // Creates the output directory if it's not made yet 
        std::filesystem::create_directories(output.parent_path());


        // Loads the user accounts to be able to login
        Login login;
        login.readFile(accountsFile.string());

        // Loads the avaliable games file
        AvailableGamesFile available;
        available.readFile(availableFile.string());

        // Loads the game collection file
        GameCollectionFile collection;
        collection.readFile(collectionFile.string());

        // Loads the users to display using the new listsaccounts transactions
        UserAccountsFile userAccounts;
        userAccounts.readFile(accountsFile.string());




        // For saving the transactions from the current session, including the end session transaction, then ending the session
        auto finish = [&]() {
            // Save the transaction log before ending the session so a failed write
            // leaves the user logged in and able to retry.
            auto a = runtime::account(runtime::current);

            // logs the end of a session
            auto log = runtime::records;
            log.push_back(runtime::accountRecord("00", runtime::current, a.type, a.balance));

            // reads the already logged transactions to add the new transaction without overwriting previous ones
            auto allRecords = std::filesystem::exists(output) ? readRecords(output.string(), 0) : std::vector<std::string>{};
            allRecords.insert(allRecords.end(), log.begin(), log.end());
            writeRecords(output.string(), allRecords, 0);

            // Ends the session after its logged
            Session s;
            s.end();

            std::cout << "Logged out" << '\n';

        };


        // Informs users that the current implementation of the program is a prototype, and the transaction file output destination.
        std::cout << "prototype.\nTransaction file: " << output.string() << "\nTransactions are saved when you log out.\n";
        std::string command; // the transaction that the user enters

        // continues to ask users for their desired transaction
        while (std::cout << "> Enter transaction: " && std::getline(std::cin, command)) {
            try {
                command = runtime::trim(command); // removes whitespace from user input
                
                // if the user doesn't enter anything, then ask again
                if (command.empty()){
                    continue;
                }

                // logs user in if they choose 
                if (command == "login") {
                    login.login();
                    std::cout << "Logged in as " << runtime::current << " (" << runtime::account(runtime::current).type << ")\n";

                    // Displays the avaliable games on login
                    GameCatalogPage c;
                    c.display(available, std::cout);
                    continue;
                }
                
                // don't accept transactions other than login if logged out
                runtime::loggedIn();

                // Logs user out and ends the session if they choose
                if (command == "logout") {
                    finish();
                    continue;
                }

                // Creates a user if the correct user type as specificied
                if (command == "create") {
                    runtime::administrator(); // only admin can create users
                    auto name = runtime::ask("New username: "); // prompts user for the new user's username
                    auto type = runtime::ask("Type (AA/FS/BS/SS): "); // prompts user for the new user's type
                    auto value = runtime::money(runtime::ask("Initial credit: ")); // prompts user for the new user's credit balance
                    
                    // creates the new user in memory, and logs the transaction
                    Create c;
                    c.addUser(name, type, value);
                } 

                // deletes user if they choose
                else if (command == "delete") {
                    runtime::administrator(); // only admin can delete users
                    auto name = runtime::ask("Username to delete: "); // prompts user for the name of the user to delete
                    auto type = runtime::account(name).type; // gets that account type

                    // deletes the user in memory, and logs the transaction
                    DeleteUser d;
                    d.removeUser(name, type);
                } 
                
                // sell games
                else if (command == "sell") {
                    Sell s;
                    require(s.hasPermission(), "Selling is not permitted."); // checks if the user has permission to sell games
                    auto title = runtime::ask("Game: "); // prompts user to enter the new game's name
                    auto value = runtime::money(runtime::ask("Price: ")); // prompts user to enter the new game's price 
                    s.execute(title, value); // publishes game which is now displayed on login and can be bought, without overwriting files (in memory)
                }
                
                // buy games
                else if (command == "buy") {
                    Buy b;
                    require(b.hasPermission(), "Buying is not permitted."); // checks if the user has permission to buy games
                    auto title = runtime::ask("Game: "); // Asks user for the game they want to buy
                    auto seller = runtime::ask("Seller: "); // Asks user for the name of the seller selling the game
                    b.execute(title, seller); // buys the game, saves it with the user in memory, and logs the transaction
                }
                // refunding users
                else if (command == "refund") {
                    runtime::administrator(); // only admin can refund users
                    auto buyer = runtime::ask("Buyer: "); // asks user for the buyer to refund
                    auto seller = runtime::ask("Seller: "); // asks user for the seller to refund from
                    auto value = runtime::money(runtime::ask("Amount: "));
                    Refund r;
                    r.execute(buyer, seller, value); // refunds the users, updates balance, and logs the transaction
                }
                // adding credit
                else if (command == "addcredit") {
                    // if the user doing the transaction is an admin, then ask for the username of the user to add credit to.
                    // otherwise, credits will be added to the current user.
                    auto user = runtime::admin() ? runtime::ask("Username: ") : runtime::current;
                    auto value = runtime::money(runtime::ask("Amount: ")); // asks user for the amount to add
                    AddCredit a;
                    a.updateCredit(user, value); // updates the balance of the user, and logs the transaction
                }

                // lists the current active accounts
                else if (command == "listaccounts") {
                    runtime::administrator(); // only admin can do this transaction
                    
                    // displays the existing users
                    ListAccounts l;
                    l.display(userAccounts, std::cout);
                    continue;
                } else
                    throw std::runtime_error("Unknown command"); // handles if the user enters something that isn't an existing transaction
                std::cout << "Done.\n";
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << '\n';
            }
        }
        if (runtime::active)
            finish();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
