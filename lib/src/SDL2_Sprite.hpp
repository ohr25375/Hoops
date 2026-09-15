#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "SDL2_Sprite/SDL2_Sprite_ColorPalette.hpp"

struct SPRITE
{
    int width;
    int height;
    std::vector<std::string> sprite;
    
    SPRITE(std::vector<std::string> sprite);

    void cleanSprite();
};