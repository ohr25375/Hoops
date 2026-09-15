#pragma once

#include <string>
#include <vector>
#include <array>

namespace hexManip {
    std::string toHex(uint64_t data, int length, bool fill = true);
    std::string toAscii(std::vector<uint8_t>::iterator& it, int length, bool advance = true);
    int toInt(std::vector<uint8_t>::iterator& it, int length, bool advance = true);
    int toIntRev(std::vector<uint8_t>::iterator& it, int length, bool advance = true);
    uint32_t toUint32_t(std::array<uint8_t, 4> uint8x4, bool reverse = false);
}