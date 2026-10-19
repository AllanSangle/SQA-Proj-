#include "Runtime.inc"

void GameCollectionFile::readFile(const std::string& path) {
    runtime::loadOwned(path);
    gameCollection = runtime::owned;
}

void GameCollectionFile::writeFile(const std::string& path) const {
    runtime::saveLog(path);
}

bool GameCollectionFile::ownsGame(const std::string& user, const std::string& game) const {
    return runtime::owns(user, game);
}

std::vector<OwnedGame> GameCollectionFile::getOwnedGames(const std::string& user) const {
    std::vector<OwnedGame> result;
    for (const auto& o : runtime::owned)
        if (o.username == user)
            result.push_back(o);
    return result;
}
