#include "Runtime.inc"

// Reads the owned games file from user profile 
void GameCollectionFile::readFile(const std::string& path) {
    runtime::loadOwned(path);
    gameCollection = runtime::owned;
}

// writes the owned games file
void GameCollectionFile::writeFile(const std::string& path) const {
    runtime::saveLog(path);
}

// checks if the user owns the game
bool GameCollectionFile::ownsGame(const std::string& user, const std::string& game) const {
    return runtime::owns(user, game);
}

// gets the owned games
std::vector<OwnedGame> GameCollectionFile::getOwnedGames(const std::string& user) const {
    std::vector<OwnedGame> result;
    for (const auto& o : runtime::owned)
        if (o.username == user)
            result.push_back(o);
    return result;
}
