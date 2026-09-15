#include "hexManipulator.hpp"

namespace hexManipulator {
    std::string toHexString(std::byte byte, uint32_t length) {
        return toHexString(std::to_integer<uint8_t>(byte), length);
    }
} // hexManipulator