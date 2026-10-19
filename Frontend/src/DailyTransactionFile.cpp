#include "Runtime.inc"

void DailyTransactionsFile::addTransaction(const std::string& transaction) {
    runtime::records.push_back(transaction);
    transactions = runtime::records;
}

void DailyTransactionsFile::clearTransactions() {
    transactions.clear();
    runtime::records.clear();
}

void DailyTransactionsFile::writeFile(const std::string& path) const {
    runtime::saveLog(path);
}

std::string DailyTransactionsFile::accountRecord(const std::string& code, const std::string& user, const std::string& type, int64_t balance) {
    return runtime::accountRecord(code, user, type, balance);
}
