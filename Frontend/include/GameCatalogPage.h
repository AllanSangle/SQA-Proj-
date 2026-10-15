#pragma once
#include "AvailableGamesFile.h"
#include "Game.h"
#include <ostream>
class GameCatalogPage {
    std::vector<Game> games;
public:
    void load(const std::string& availableGamePath);
    void display(const AvailableGamesFile& currentGames, std::ostream& output);
};

