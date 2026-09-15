#include "color.hpp"

namespace color {
    color::color(uint32_t rgba) {
        r = (rgba >> 24) & 0xff;
        g = (rgba >> 16) & 0xff;
        b = (rgba >>  8) & 0xff;
        a = (rgba >>  0) & 0xff;
    }
    
    color::color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
    
    color::color(uint8_t mixed1, uint8_t mixed2, int rIndex, int gIndex, int bIndex, int aIndex)
    {
    }
    
    color::color(uint8_t indexed, std::vector<color> palette)
    {
    }
    
    color::color() : r(0), g(0), b(0), a(0xff) {}
    
    uint32_t color::getRGBA() const {
        return (((uint32_t)r) << 24) + (((uint32_t)g) << 16) + (((uint32_t)b) << 8) + ((uint32_t)a);
    }
    
    uint32_t color::getRGB() const {
        return (((uint32_t)r) << 16) + (((uint32_t)g) << 8) + (((uint32_t)b) << 0);
    }
    
    color::operator uint32_t() const {
        return getRGBA();
    }
}
