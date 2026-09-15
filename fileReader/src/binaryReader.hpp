#pragma once
#ifndef BINARY_READER
#define BINARY_READER

#include <cstdint>
#include <filesystem>
#include <vector>

namespace binaryReader {
    // std::vector<std::byte> getBytesFromFile(std::filesystem::path path);
    std::vector<uint8_t> getBytesFromFile(std::filesystem::path path);
    void printBytes(const std::vector<std::byte>& bytes);
    void printBytes(const std::vector<uint8_t>& bytes);
} // binaryReader

#endif