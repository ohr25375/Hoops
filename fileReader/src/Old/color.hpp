#pragma once

#include <vector>
#include <cstdint>

namespace color {
    class color {
    public:
        uint8_t r,g,b,a;

        color(uint32_t rgba);
        color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff);
        color(uint8_t mixed1, uint8_t mixed2, int rIndex, int gIndex, int bIndex, int aIndex = -1);
        color(uint8_t indexed, std::vector<color> palette);
        color();

        uint32_t getRGBA() const;
        uint32_t getRGB() const;

        operator uint32_t() const;
    };
}