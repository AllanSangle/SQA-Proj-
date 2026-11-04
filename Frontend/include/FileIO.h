#pragma once
#include <string>
#include <vector>
// Frontend/src/FileIO.inc
//helper classfor daily transaction sets permission for read and write
std::vector<std::string> readRecords(const std::string& path, std::size_t width);
void writeRecords(const std::string& path, const std::vector<std::string>& records, std::size_t width);

