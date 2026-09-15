#pragma once

#include <map>
#include "../SDL2_Addon.hpp"

const char PALETTE_EMPTY = ' ';

class COLOR_PALETTE
{
public:
    COLOR_PALETTE(std::map<char, SDL2Addon::SDL_COLOR> palette)
    {
        this->palette = palette;
    }

    SDL2Addon::SDL_COLOR getColor(const char &key) const
    {
        if (palette.count(key))
        {
            return palette.at(key);
        }
        return SDL2Addon::SDL_COLOR();
    }

private:
    std::map<char, SDL2Addon::SDL_COLOR> palette;
};

COLOR_PALETTE colorPalette = {
    {
        {' ', SDL2Addon::SDL_COLOR(0,0)},
        {'R', SDL2Addon::SDL_COLOR(0xff0000,0)},
        {'r', SDL2Addon::SDL_COLOR(0x800000,0)},
        {'G', SDL2Addon::SDL_COLOR(0x00ff00,0)},
        {'g', SDL2Addon::SDL_COLOR(0x008000,0)},
        {'B', SDL2Addon::SDL_COLOR(0x0000ff,0)},
        {'b', SDL2Addon::SDL_COLOR(0x000080,0)},
        {'C', SDL2Addon::SDL_COLOR(0x00ffff,0)},
        {'c', SDL2Addon::SDL_COLOR(0x008080,0)},
        {'M', SDL2Addon::SDL_COLOR(0xff00ff,0)},
        {'m', SDL2Addon::SDL_COLOR(0x800080,0)},
        {'Y', SDL2Addon::SDL_COLOR(0xffff00,0)},
        {'y', SDL2Addon::SDL_COLOR(0x808000,0)},
        {'W', SDL2Addon::SDL_COLOR(0xffffff,0)},
        {'w', SDL2Addon::SDL_COLOR(0xa0a0a0,0)},
        {'x', SDL2Addon::SDL_COLOR(0x505050,0)},
        {'X', SDL2Addon::SDL_COLOR(0x000000,0)},

        {'O', SDL2Addon::SDL_COLOR(0xffa000,0)},
        {'o', SDL2Addon::SDL_COLOR(0x805000,0)},
        {'U', SDL2Addon::SDL_COLOR(0xff00a0,0)},
        {'u', SDL2Addon::SDL_COLOR(0x800050,0)},
        {'N', SDL2Addon::SDL_COLOR(0xa0ff00,0)},
        {'n', SDL2Addon::SDL_COLOR(0x508000,0)},
        {'S', SDL2Addon::SDL_COLOR(0x00ffa0,0)},
        {'s', SDL2Addon::SDL_COLOR(0x008050,0)},
        {'V', SDL2Addon::SDL_COLOR(0xa000ff,0)},
        {'v', SDL2Addon::SDL_COLOR(0x508080,0)},
        {'A', SDL2Addon::SDL_COLOR(0x00a0ff,0)},
        {'a', SDL2Addon::SDL_COLOR(0x005080,0)},
    }
};