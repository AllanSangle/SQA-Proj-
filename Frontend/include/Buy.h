#pragma once
#include <cstdint>
#include "Session.h"
#include "UserAccountFiles.h"
#include "AvailableGamesFile.h"
#include "GameCollectionFile.h"
#include "DailyTransactionFile.h"
class Buy {
private:
    int64_t price = 0;
    std::string buyer;
    std::string seller;
    std::string game;
public:
    void execute(const std::string& game, const std::string& seller);
    bool hasPermission();
    int64_t getPrice();
    void addGame(std::string buyer, std::string game);
    bool gameAlreadyOwned(); 
    std::string logBuy(const std::string& code, const std::string& game, const std::string& seller, const std::string& buyer, int64_t price);
};
