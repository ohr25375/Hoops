#pragma once

#include <type_traits>
#include <sstream>
#include <bitset>
#include <iomanip>

namespace HEX_CONVERTER {
    template <typename T, typename = std::enable_if<std::is_integral<T>::value>>
    std::string toHex(const T value, uint32_t length = 0) {
        std::stringstream ss;
        if (length < sizeof(value) * 2) length = sizeof(value) * 2;
        ss << std::setfill('0') << std::setw(length) << std::hex << (uint32_t)value;
        return ss.str();
    }
    template <typename T, typename = std::enable_if<std::is_integral<T>::value>>
    std::string toBin(const T value, uint32_t length = 0) {
        if (length < sizeof(value) * 8) length = sizeof(value) * 8;
        return std::bitset<64>(value).to_string().substr(64 - length);
    }
}