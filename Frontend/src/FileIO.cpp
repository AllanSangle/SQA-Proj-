#include "Runtime.inc"

std::vector<std::string> readRecords(const std::string& path, std::size_t width) {
    std::ifstream input(path);
    require(bool(input), "Cannot open: " + path);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty())
            continue;
        require(!width || line.size() == width, "Incorrect record width in " + path + ": expected " + std::to_string(width));
        lines.push_back(line);
    }
    require(!input.bad(), "Read failed: " + path);
    return lines;
}

void writeRecords(const std::string& path, const std::vector<std::string>& records, std::size_t width) {
    for (const auto& r : records)
        require(!width || r.size() == width, "Incorrect output record width.");
    std::ofstream output(path);
    require(bool(output), "Cannot write: " + path);
    for (const auto& r : records)
        output << r << '\n';
    output.close();
    require(bool(output), "Write failed: " + path);
}
