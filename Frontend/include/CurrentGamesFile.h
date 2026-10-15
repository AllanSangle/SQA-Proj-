#pragma once
#include "OwnedGame.h"
#include <vector>
class CurrentGamesFile {
private:
    std::vector<OwnedGame> gameCollection;
public:
    void readFile(const std::string& path);
    void writeFile(const std::string& path) const;
    bool ownsGame(const std::string& username, const std::string& game) const;
    std::vector<OwnedGame> getOwnedGames(const std::string& username) const;
};

