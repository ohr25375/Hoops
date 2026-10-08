#include "SDL2Util_ColorWrapper.hpp"

namespace SDL2Util {
    Color::Color(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    Color::Color(const unsigned int rgb888, const Uint8 a) {
        this->r = (rgb888 & 0xff0000) >> 16;
        this->g = (rgb888 & 0x00ff00) >> 8;
        this->b = (rgb888 & 0x0000ff);
        this->a = a;
    }

    Color::Color() {
        r    = 0;
        g    = 0;
        b    = 0;
        a    = 255;
    }

    Color Color::darken(double percentage) {
        Color res  = *this;
        res.r         *= percentage;
        res.g         *= percentage;
        res.b         *= percentage;
        return res;
    }

    Color::operator SDL_Color() const {
        return {r, g, b, a};
    }

    uint16_t Color::getRGB444() const {
        uint16_t res =
        (((uint16_t)r & 0xf0) << 4) |
        (((uint16_t)g & 0xf0) << 0) |
        (((uint16_t)b & 0xf0) >> 4);
        return res;
    }
    
    Color createColorFromRGB444(const uint16_t rgb444) {
        uint8_t r = ((rgb444 >> 8) & 0xf) | (((rgb444 >> 8) & 0xf) << 4);
        uint8_t g = ((rgb444 >> 4) & 0xf) | (((rgb444 >> 4) & 0xf) << 4);
        uint8_t b = ((rgb444 >> 0) & 0xf) | (((rgb444 >> 0) & 0xf) << 4);
        return Color(r,g,b);
    }
}