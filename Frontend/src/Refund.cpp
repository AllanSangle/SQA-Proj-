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
    auto& b = runtime::account(user);
    auto& s = runtime::account(vendor);
    require(value > 0 && s.balance >= value, "Invalid refund or insufficient seller credit."); // refund amount has to be a valid amount, and sellers have to have enough credits
    require(value <= MAX_CREDIT - b.balance, "Buyer balance limit exceeded."); // makes sure refunds can't make users exceed the max cred limit
    buyer = user;
    seller = vendor;
    price = value;
    b.balance += value;
    runtime::records.push_back(logRefund("05", buyer, seller, price)); // logs refund transaction
}

std::string Refund::logRefund(const std::string& code, const std::string& user, const std::string& vendor, int64_t value) {
    return runtime::field(code, 2) + " " + runtime::field(user, 15) + " " + runtime::field(vendor, 15) + " " + runtime::amountField(value, 9);
}
