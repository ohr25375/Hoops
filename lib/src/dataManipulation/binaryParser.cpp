#include "binaryParser.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include "hexConverter.hpp"

namespace BINARY_PARSER {
    std::string getBinaryAsHexString(const uint8_t* data, const std::size_t size, const std::size_t offset = 0) {
        std::stringstream ss;
        ss << "Length : " << size << '\n';

        for (int32_t i = offset; i < offset + size; i++) {
            if (i % 16 == 0) {
                ss << HEX_CONVERTER::toHex(i, 8) << ": ";
            }
            ss << HEX_CONVERTER::toHex(data[i], 2) << (i % 2 == 1 ? " " : "");
            if (i % 16 != 15) continue;

            ss << " ";
            for (int32_t j = 15; j >= 0; j--) {
                if (data[i - j] < 33 || data[i - j] > 126) {
                    ss << '.';
                    continue;
                }
                ss << data[i - j];
            }
            ss << '\n';
        }
        if (size % 16 != 0) {
            ss << '\n';
        }
        return ss.str();
    }

    void printBinaryAsHex(const uint8_t* data, const std::size_t size, const std::size_t offset = 0) {
        std::cout << getBinaryAsHexString(data, size, offset);
    }
}