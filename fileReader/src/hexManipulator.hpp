#pragma once
#ifndef HEX_MANIPULATOR
#define HEX_MANIPULATOR

#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>

namespace hexManipulator {
    template <typename T, typename = std::enable_if<std::is_integral<T>::value>>
    std::string toHexString(T value, uint32_t length = 0) {
        std::stringstream ss;
        if (length < sizeof(value) * 2) length = sizeof(value) * 2;
        ss << std::setfill('0') << std::setw(length) << std::hex << (uint32_t)value;
        return ss.str();
    }

    std::string toHexString(std::byte byte, uint32_t length);

    template <typename T, typename = std::enable_if<std::is_integral<T>::value>>
    void packBytes(T value, std::vector<uint8_t>& vector) {
        uint32_t size = (uint32_t)(sizeof(value));
        std::vector<uint8_t> res(size);
        for (uint32_t i = 0; i < size; i++) {
            res[i] = (value >> (8 * i)) & 0xff;
        }
        vector.insert(std::end(vector), std::begin(res), std::end(res));
    }

    template <typename T, typename = std::enable_if<std::is_integral<T>::value>>
    void packBytes(T value, size_t bitSize, std::vector<uint8_t>& vector) {
        int size = bitSize / 8;
        std::vector<uint8_t> res(size);
        for (uint32_t i = 0; i < size; i++) {
            res[i] = (value >> (8 * i)) & 0xff;
        }
        vector.insert(std::end(vector), std::begin(res), std::end(res));
    }

    template<typename T>
    T getValue(std::vector<uint8_t>::iterator& vector, bool isAdvancing = true) {
        uint32_t size = (uint32_t)(sizeof(T));
        T res = 0;
        for (uint32_t i = 0; i < size; i++) {
            res += ((T)*(vector + i)) << (8 * i);
        }
        if (isAdvancing) {
            vector += size;
        }
        return res;
    }
} // hexManipulator

#endif