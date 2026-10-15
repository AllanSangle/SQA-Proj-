#pragma once 
#include <cstdint>
#include <string>
using Credit = int64_t;

const Credit MAX_CREDIT = 1000000;
const Credit Max_SESSION_CREDIT = 1000000;
const Credit MAX_PRICE  = 99999;

class Credit {
public:
    Credit(int value) : value(value) {}
    int value;
    void require(bool condition, const std::string& message);
    void validateName(const std::string& name, std::size_t maximum);
};