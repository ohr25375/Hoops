#include "color.hpp"

namespace COLOR {
    COLOR::COLOR() : r(0), g(0), b(0), a(0xff) {}
    COLOR::COLOR(uint32_t rgba) : COLOR(rgba >> 8, rgba & 0xff) {}
    COLOR::COLOR(uint32_t rgb, uint8_t a) : a(a) {
        r = (rgb >> 16) & 0xff;
        g = (rgb >>  8) & 0xff;
        b = (rgb >>  0) & 0xff;
    }
    COLOR::COLOR(uint8_t r, uint8_t g, uint8_t b) : COLOR(r,g,b,0xff) {}
    COLOR::COLOR(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
    uint32_t COLOR::rgb() const {
        uint32_t res = r;
        res = (res << 8) + g;
        res = (res << 8) + b;
        return res;
    }
    uint32_t COLOR::rgba() const {
        uint32_t res = rgb();
        res = (res << 8) + a;
        return res;
    }
}