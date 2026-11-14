#include "Runtime.inc"

// Creates a game with a name, seller, and price.
Game::Game(std::string name, std::string seller, int64_t price)
    : name(std::move(name)), seller(std::move(seller)), price(price) {
    validateName(this->name, 25);
    validateName(this->seller, 15);
    require(price >= 0 && price <= MAX_PRICE, "Invalid game price.");
}

// Returns the name of the game.
const std::string& Game::getName() const {
    return name;
}

// Returns the seller of the game.
const std::string& Game::getSeller() const {
    return seller;
}

// Returns the price of the game.
int64_t Game::getPrice() const {
    return price;
}
