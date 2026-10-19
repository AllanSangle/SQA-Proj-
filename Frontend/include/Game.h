#pragma once
#include <cstdint>
#include <string>
#include <string>

class Game {
private:
    std::string name;
    std::string seller;
    int64_t price;
public:
    Game(std::string name, std::string seller, int64_t price);
    const std::string& getName() const;
    const std::string& getSeller() const;
    int64_t getPrice() const;
};

