#include "Runtime.inc"

bool Refund::hasPermission() {
    return runtime::admin();
}

int64_t Refund::getPrice() {
    return price;
}

// executes the refund
void Refund::execute(const std::string& user, const std::string& vendor, int64_t value) {
    runtime::administrator(); // only admin can refund
    require(user != vendor, "Buyer and seller must differ."); // makes the user and seller aren't the same user
    require(runtime::accounts.count(user) && runtime::accounts.count(vendor), "Failed to refund. Account with that username does not exist.");// Requires the buyer to own a game listed by this seller.
    require(std::any_of(runtime::owned.begin(), runtime::owned.end(), [&](const OwnedGame& o) {
        auto g = runtime::games.find(o.game);
        return o.username == user && g != runtime::games.end() && g->second.getSeller() == vendor;
    }), "Failed to refund. This user does not own a game from that seller.");// Gets both accounts so their balances can be updated together.
    auto& b = runtime::account(user);
    auto& s = runtime::account(vendor);
    require(value > 0 && s.balance >= value, "Failed to refund. Seller doesn't have enough credits to refund buyer."); // refund amount has to be a valid amount, and sellers have to have enough credits
    require(value <= MAX_CREDIT - b.balance, "Failed to refund. Buyer will exceed credit limit."); // makes sure refunds can't make users exceed the max cred limit
    buyer = user;
    seller = vendor;
    price = value;
    b.balance += value;
    s.balance -= value;
    runtime::records.push_back(logRefund("05", buyer, seller, price)); // logs refund transaction
}

// Formats the refund record with its code, buyer, seller, and amount.
std::string Refund::logRefund(const std::string& code, const std::string& user, const std::string& vendor, int64_t value) {
    return runtime::field(code, 2) + " " + runtime::field(user, 15) + " " + runtime::field(vendor, 15) + " " + runtime::amountField(value, 9);
}
