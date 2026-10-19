#pragma once
#include <cstdint>
#include "Session.h"
#include "UserAccountFiles.h"
#include "AvailableGamesFile.h"
#include "GameCollectionFile.h"
#include "DailyTransactionFile.h"
class Sell {
private:
    int64_t price = 0;
    std::string seller;
    std::string game;
public:
    void execute(const std::string& game, int64_t price);
    bool hasPermission();
    bool gameAlreadyExists(const std::string& game);
    std::string logSell(const std::string& code, const std::string& game, const std::string& seller, int64_t price);
};
