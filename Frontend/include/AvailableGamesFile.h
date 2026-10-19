#pragma once
#include <cstdint>
#include "Game.h"
#include <vector>

class AvailableGamesFile {
    std::vector<Game> games;
public:
    void readFile(const std::string& path);
    void writeFile(const std::string& path) const;
    Game addGame(const std::string& game, const std::string& seller, int64_t price);
    const Game* findGame(const std::string& game) const;
};

