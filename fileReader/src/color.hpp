#pragma once
#ifndef COLOR_DEFINE
#define COLOR_DEFINE

#include <cstdint>

namespace COLOR {
    class COLOR {
    public:
        uint8_t r,g,b,a;
        COLOR();
        COLOR(uint32_t rgba);
        COLOR(uint32_t rgb, uint8_t a);
        COLOR(uint8_t r, uint8_t g, uint8_t b);
        COLOR(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

        uint32_t rgb() const;
        uint32_t rgba() const;
    };
}

#endif