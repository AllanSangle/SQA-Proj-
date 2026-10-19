#include "Runtime.inc"

void GameCatalogPage::load(const std::string& path) {
    AvailableGamesFile f;
    f.readFile(path);
    games.clear();
    for (const auto& g : runtime::games)
        games.push_back(g.second);
}

void GameCatalogPage::display(const AvailableGamesFile&, std::ostream& out) {
    for (const auto& item : runtime::games) {
        const auto& g = item.second;
        out << g.getName() << " | " << g.getSeller() << " | $" << runtime::moneyText(g.getPrice());
        if (runtime::newGames.count(g.getName()))
            out << " (available next session)";
        out << '\n';
    }
}
