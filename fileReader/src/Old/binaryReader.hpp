#pragma once

#include <vector>
#include <filesystem>

namespace binaryReader {
    std::vector<uint8_t> readBinary(std::filesystem::path path);
    void printBinary(std::vector<uint8_t> data);
}
