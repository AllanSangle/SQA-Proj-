#include "Runtime.inc"

bool ListAccounts::hasPermission() {
    return runtime::admin();
}

// Displays the list of users
void ListAccounts::display(const UserAccountsFile&, std::ostream& out) {
    runtime::administrator(); // only admin can see a list of users
    out << "Active Accounts:\n";

    // for printing the users, their type, and balance
    for (const auto& item : runtime::accounts) {
        out << runtime::field(item.first, 15) << ' ' << item.second.type << ' ' << runtime::amountField(item.second.balance, 9) << '\n';
    }
}