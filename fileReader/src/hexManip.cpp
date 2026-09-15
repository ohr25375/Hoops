#include "hexManip.hpp"

#include <sstream>
#include <iomanip>

namespace hexManip {
    std::string toHex(uint64_t data, int length, bool fill) {
        std::stringstream ss;
        if (fill) {
            ss << std::setfill('0') << std::setw(length) << std::hex << data;
        } else {
            ss << std::hex << data;
        }
        return ss.str();
    }
    
    std::string toAscii(std::vector<uint8_t>::iterator &it, int length, bool advance) {
        std::string res;
        for (int i = 0; i < length; i++) {
            res += (char)*(it + i);
        }
        if (advance) {
            std::advance(it, length);
        }
        return res;
    }
    
    int toInt(std::vector<uint8_t>::iterator &it, int length, bool advance) {
        int res = 0;
        for (int i = 0; i < length; i++) {
            res <<= 8;
            res += (int)*(it + i);
        }
        if (advance) {
            std::advance(it, length);
        }
        return res;
    }
    
    int toIntRev(std::vector<uint8_t>::iterator &it, int length, bool advance) {
        int res = 0;
        for (int i = 0; i < length; i++) {
            res += ((int)*(it + i)) << (8 * i);
        }
        if (advance) {
            std::advance(it, length);
        }
        return res;
    }
    
    uint32_t toUint32_t(std::array<uint8_t, 4> uint8x4, bool reverse) {
        uint32_t res = 0;
        for (int i = 0; i < 4; i++) {
            int index = reverse ? 4 - i - 1 : i;
            res += uint8x4[index] << index * 8;
        }
        return res;
    }
}