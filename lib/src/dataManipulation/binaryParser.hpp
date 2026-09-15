#pragma once

#include <cstdint>
#include <string>

namespace BINARY_PARSER {
    std::string getBinaryAsHexString(const uint8_t* data, const std::size_t size, const std::size_t offset);
    void printBinaryAsHex(const uint8_t* data, const std::size_t size, const std::size_t offset);
}