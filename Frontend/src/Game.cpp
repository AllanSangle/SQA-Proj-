#include "Runtime.inc"

Game::Game(std::string name, std::string seller, int64_t price)
    : name(std::move(name)), seller(std::move(seller)), price(price) {
    validateName(this->name, 25);
    validateName(this->seller, 15);
    require(price >= 0 && price <= MAX_PRICE, "Invalid game price.");
}

const std::string& Game::getName() const {
    return name;
}

const std::string& Game::getSeller() const {
    return seller;
}

int64_t Game::getPrice() const {
    return price;
}
