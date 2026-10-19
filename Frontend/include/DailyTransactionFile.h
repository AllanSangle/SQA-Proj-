#pragma once
#include <cstdint>
#include <string>
#include <vector>
class DailyTransactionsFile {
    std::vector<std::string> transactions;
public:
    void addTransaction(const std::string& transaction);
    void clearTransactions();
    void writeFile(const std::string& path) const;
    static std::string accountRecord(const std::string& code, const std::string& user, const std::string& type, int64_t balance);
};

