#pragma once
#include "Credit.h"
#include <string>

struct Gamelist {
    std::string name;
    std::string seller;
    Credit price;
};

class Game {
private:
    std::string name;
    std::string seller;
    Credit price;
public:
    Game(std::string name, std::string seller, Credit price);
    const std::string& getName() const;
    const std::string& getSeller() const;
    Credit getPrice() const;
};

