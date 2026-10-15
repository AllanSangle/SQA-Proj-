#pragma once
#include "CurrentGamesFile.h"
class GameLibrary {
private:
    std::string username;
    std::vector<OwnedGame> ownedGames;
public:
    const std::vector<OwnedGame>& getOwnedGames(const std::string& currentGamesPath, const std::string& username);
};

