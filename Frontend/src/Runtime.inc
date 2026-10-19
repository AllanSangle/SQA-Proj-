// Internal implementation support, shared by the .cpp files. C++17 inline state.
#pragma once
#include <cstdint>
#include <string>
#include "Session.h"
#include "UserAccounts.h"
#include "UserAccountFiles.h"
#include "AvailableGamesFile.h"
#include "GameCollectionFile.h"
#include "DailyTransactionFile.h"
#include "FileIO.h"
#include "Login.h"
#include "Logout.h"
#include "AddCredit.h"
#include "Buy.h"
#include "Sell.h"
#include "Refund.h"
#include "Create.h"
#include "DeleteUser.h"
#include "GameCatalogPage.h"
#include "ListAccounts.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <utility>
inline constexpr int64_t MAX_CREDIT = 99999900;
inline constexpr int64_t Max_SESSION_CREDIT = 100000;
inline constexpr int64_t MAX_PRICE = 99999;

inline void require(bool condition, const std::string& message) {
    if (!condition)
        throw std::runtime_error(message);
}

inline void validateName(const std::string& name, std::size_t maximum) {
    require(!name.empty() && name.size() <= maximum, "Name must contain 1 to " + std::to_string(maximum) + " characters.");
    require(name != "END" && name.front() != ' ' && name.back() != ' ' && name.find_first_of("\r\n\t") == std::string::npos, "Invalid or reserved name.");
}

namespace runtime {
    struct Account {
        std::string type;
        int64_t balance = 0;
    };

    inline std::map<std::string, Account> accounts;
    inline std::map<std::string, Game> games;
    inline std::vector<OwnedGame> owned;
    inline std::vector<std::string> records;
    inline std::map<std::string, int64_t> added;
    inline std::set<std::string> newGames;
    inline std::string current;
    inline bool active = false;

    inline std::string trim(std::string s) {
        auto a = s.find_first_not_of(" \r\n\t");
        if (a == std::string::npos)
            return "";
        return s.substr(a, s.find_last_not_of(" \r\n\t") - a + 1);
    }

    inline int64_t money(const std::string& input) {
        std::string s = trim(input);
        require(!s.empty(), "Enter a monetary amount.");
        auto dot = s.find('.');
        require(dot == std::string::npos || s.find('.', dot + 1) == std::string::npos, "Invalid amount.");
        std::string whole = s.substr(0, dot), fraction = dot == std::string::npos ? "" : s.substr(dot + 1);
        require(!whole.empty() && whole.size() <= 6 && fraction.size() <= 2, "Amount must have at most six whole digits and two decimal places.");
        require(std::all_of(whole.begin(), whole.end(), [](unsigned char c) {
                    return c >= '0' && c <= '9';
                }) &&
                    std::all_of(fraction.begin(), fraction.end(), [](unsigned char c) {
                        return c >= '0' && c <= '9';
                    }),
                "Invalid nonnegative amount.");
        while (fraction.size() < 2)
            fraction += '0';
        return std::stoll(whole) * 100 + std::stoll(fraction);
    }

    inline std::string moneyText(int64_t n) {
        require(n >= 0, "Negative monetary value.");
        std::ostringstream out;
        out << n / 100 << '.' << std::setw(2) << std::setfill('0') << n % 100;
        return out.str();
    }

    inline std::string field(const std::string& s, std::size_t n) {
        require(s.size() <= n, "Field is too long: " + s);
        return s + std::string(n - s.size(), ' ');
    }

    inline std::string amountField(int64_t n, std::size_t width) {
        auto s = moneyText(n);
        require(s.size() <= width, "Amount exceeds field width.");
        return std::string(width - s.size(), '0') + s;
    }

    inline Account& account(const std::string& name) {
        auto i = accounts.find(name);
        require(i != accounts.end(), "Unknown user: " + name);
        return i->second;
    }

    inline bool admin() {
        return active && account(current).type == "AA";
    }

    inline void loggedIn() {
        require(active, "Please log in first.");
    }

    inline void administrator() {
        loggedIn();
        require(admin(), "Administrator permission required.");
    }

    inline bool owns(const std::string& user, const std::string& game) {
        return std::any_of(owned.begin(), owned.end(), [&](const OwnedGame& o) {
            return o.username == user && o.game == game;
        });
    }

    inline std::string ask(const std::string& prompt) {
        std::cout << prompt;
        std::string s;
        if (!std::getline(std::cin, s))
            throw std::runtime_error("Input ended.");
        return trim(s);
    }

    inline void loadAccounts(const std::string& path) {
        std::map<std::string, Account> loaded;
        for (const auto& line : readRecords(path, 28)) {
            std::string name = trim(line.substr(0, 15));
            if (name == "END")
                break;
            validateName(name, 15);
            std::string type = line.substr(16, 2);
            require(type == "AA" || type == "FS" || type == "BS" || type == "SS", "Invalid account type.");
            require(!loaded.count(name), "Duplicate account name.");
            loaded.emplace(name, Account{type, money(line.substr(19, 9))});
        }
        accounts = std::move(loaded);
    }

    inline void loadGames(const std::string& path) {
        std::map<std::string, Game> loaded;
        for (const auto& line : readRecords(path, 0)) {
            auto name = trim(line.substr(0, 25));
            if (name == "END")
                break;
            require(line.size() == 49, "Invalid game record width.");
            Game g(name, trim(line.substr(27, 15)), money(line.substr(43, 6)));
            require(!loaded.count(name), "Duplicate game name.");
            loaded.emplace(name, g);
        }
        games = std::move(loaded);
    }

    inline void loadOwned(const std::string& path) {
        std::vector<OwnedGame> loaded;
        for (const auto& line : readRecords(path, 0)) {
            auto game = trim(line.substr(0, 25));
            if (game == "END")
                break;
            require(line.size() == 42, "Invalid ownership record width.");
            loaded.push_back({trim(line.substr(27, 15)), game});
        }
        owned = std::move(loaded);
    }

    inline void saveLog(const std::string& path) {
        writeRecords(path, records, 0);
    }

    inline std::string accountRecord(const std::string& code, const std::string& user, const std::string& type, int64_t balance) {
        return field(code, 2) + " " + field(user, 15) + " " + field(type, 2) + " " + amountField(balance, 9);
    }
} // namespace runtime
