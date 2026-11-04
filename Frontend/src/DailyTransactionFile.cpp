#include "Runtime.inc"

// Frontend/src/DailyTransactionFile.cpp
void DailyTransactionsFile::addTransaction(const std::string& transaction) {
    runtime::records.push_back(transaction);
    transactions = runtime::records;
}

// clears all transactions and records Frontend/src/DailyTransactionFile.cpp
void DailyTransactionsFile::clearTransactions() {
    transactions.clear();
    runtime::records.clear();
}

// writes the transactions file
void DailyTransactionsFile::writeFile(const std::string& path) const {
    runtime::saveLog(path);
}


// returns the account record
std::string DailyTransactionsFile::accountRecord(const std::string& code, const std::string& user, const std::string& type, int64_t balance) {
    return runtime::accountRecord(code, user, type, balance);
}
