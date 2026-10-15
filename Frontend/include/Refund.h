#pragma once
#include "Session.h"
#include "UserAccountFiles.h"
#include "AvailableGamesFile.h"
#include "CurrentGamesFile.h"
#include "DailyTransactionFile.h"
class Refund {
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
    void UpdateCredit(std::string buyer, Credit Amount);
    std::string logRefund(const std::string& code, const std::string& user, const std::string& type, Credit balance);
};
