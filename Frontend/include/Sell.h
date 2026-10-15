#pragma once
#include "Session.h"
#include "UserAccountFiles.h"
#include "AvailableGamesFile.h"
#include "CurrentGamesFile.h"
#include "DailyTransactionFile.h"
class Sell {
private:
    Credit price;
    std::string seller;
    std::string game;
public:
    bool hasPermission();
    void readFile(const std::string& path);
    void writeFile(const std::string& path) const;
    bool gameAlreadyExists(const std::string& game);
    std::string logSell(const std::string& code, const std::string& user, const std::string& type, Credit balance);
};
