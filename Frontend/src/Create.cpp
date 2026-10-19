#include "Runtime.inc"

bool Create::isAdmin() {
    return runtime::admin();
}

// checks if the user being created already exists
bool Create::userAlreadyExists(const std::string& user) {
    return runtime::accounts.count(user) != 0;
}

// adding the user
void Create::addUser(std::string user, std::string role, int64_t credit) {
    runtime::administrator(); // only admin can create
    validateName(user, 15); // ensure the new user has a valid username length
    require(!userAlreadyExists(user), "User already exists."); // check if a user already exists with that name
    require(role == "AA" || role == "FS" || role == "BS" || role == "SS", "Type must be AA, FS, BS, or SS."); // make sure the user type is valid
    require(credit >= 0 && credit <= MAX_CREDIT, "Invalid credit.");
    username = user;
    type = role;
    runtime::accounts.emplace(user, runtime::Account{role, credit}); // adds the user in memory wihtout editing files
    runtime::records.push_back(logCreate("01", user, role, credit)); // logs the user creation transaction
}

// for logging the transaction of creating user
std::string Create::logCreate(const std::string& code, const std::string& user, const std::string& role, int64_t credit) {
    return runtime::accountRecord(code, user, role, credit);
}
