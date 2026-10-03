#pragma once
#include <SDL.h>

namespace SDL2Util {
    struct Color {
        Uint8 r, g, b, a;
        Color(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a = 255);
        Color(const unsigned int rgb888, const Uint8 a = 255);
        Color();

        Color darken(double percentage);
        operator SDL_Color() const;
        uint16_t getRGB444() const;
    };

    Color createColorFromRGB444(const uint16_t rgb444);
}