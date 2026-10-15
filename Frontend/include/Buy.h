#pragma once
#include "Session.h"
#include "UserAccountFiles.h"
#include "AvailableGamesFile.h"
#include "CurrentGamesFile.h"
#include "DailyTransactionFile.h"
class Buy {
private:
    Credit price;
    std::string buyer;
    std::string seller;
    std::string game;
public:
    bool hasPermission();
    int64_t getPrice();
    void readFile(const std::string& path);
    void writeFile(const std::string& path) const;
    void addGame(std::string buyer, std::string game);
    bool gameAlreadyOwned(); 
    std::string logBuy(const std::string& code, const std::string& user, const std::string& type, Credit balance);
};
