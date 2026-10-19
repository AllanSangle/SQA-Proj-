#pragma once
#include <cstdint>
#include "Session.h"
#include "UserAccountFiles.h"
#include "AvailableGamesFile.h"
#include "GameCollectionFile.h"
#include "DailyTransactionFile.h"
class Refund {
private:
    int64_t price = 0;
    std::string buyer;
    std::string seller;
    std::string game;
public:
    void execute(const std::string& buyer, const std::string& seller, int64_t amount);
    bool hasPermission();
    int64_t getPrice();
    std::string logRefund(const std::string& code, const std::string& buyer, const std::string& seller, int64_t amount);
};
