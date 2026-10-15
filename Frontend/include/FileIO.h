#pragma once
#include <string>
#include <vector>
std::vector<std::string> readRecords(const std::string& path, std::size_t width);
void writeRecords(const std::string& path, const std::vector<std::string>& records, std::size_t width);

