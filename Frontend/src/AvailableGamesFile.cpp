#include "Runtime.inc"

// Reads the available games file
void AvailableGamesFile::readFile(const std::string& path) {
    runtime::loadGames(path);
    games.clear();
    for (const auto& g : runtime::games) {
        games.push_back(g.second);
    }
}

// Writes the available games file
void AvailableGamesFile::writeFile(const std::string& path) const {
    runtime::saveLog(path);
}


// Adds a game
Game AvailableGamesFile::addGame(const std::string& name, const std::string& seller, int64_t price) {
    require(!runtime::games.count(name), "Game already exists.");
    Game g(name, seller, price);
    runtime::games.emplace(name, g);
    games.push_back(g);
    return g;
}

// Finds a game
const Game* AvailableGamesFile::findGame(const std::string& name) const {
    auto i = runtime::games.find(name);
    return i == runtime::games.end() ? nullptr : &i->second;
}
