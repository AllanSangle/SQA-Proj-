#include "Runtime.inc"

// Reads the available games file and refreshes the pages game list
void GameCatalogPage::load(const std::string& path) {
    AvailableGamesFile f;
    f.readFile(path);
    games.clear();
    for (const auto& g : runtime::games)
        games.push_back(g.second);
}

// Displays the current in-memory catalog, including changes from earlier transactions
void GameCatalogPage::display(const AvailableGamesFile&, std::ostream& out) {
    out << "Available Games:\n";
    for (const auto& item : runtime::games) {
        const auto& g = item.second;
        out << runtime::field(g.getName(), 26) << " " << runtime::field(g.getSeller(), 15) << " " << runtime::amountField(g.getPrice(), 6);
        if (runtime::newGames.count(g.getName()))
            out << " (available next session)";
        out << '\n';
    }
}
