#pragma once

#include <vector>
#include <SDL.h>
#include <cstdint>
#include <array>
#include "src/vector2.hpp"
#include "src/SDL2_Addon.hpp"

struct SPRITE_ROW {
    const uint8_t color;
    const uint8_t alpha;
    SPRITE_ROW(const uint8_t color = 0, const uint8_t alpha = 0) : color(color), alpha(alpha) {}
};

typedef std::array<SPRITE_ROW, 8> SPRITE;

enum struct GAME_SPRITES {
    NONE,
    BLANK,
    CURSOR,
    HOOP_001,
    HOOP_010,
    HOOP_100,
    CONNECTOR_RIGHT,
    CONNECTOR_DOWN,
    CONNECTOR_LEFT,
    CONNECTOR_UP,
    HOOP_001_GHOST,
    HOOP_010_GHOST,
    HOOP_100_GHOST,
    WALL,
    MAN,
    MAN_HAPPY,
};

struct SPRITE_DATA {
    std::vector<SDL_Point> whites{};
    std::vector<SDL_Point> blacks{};
};

SPRITE_DATA getGameSprite(const GAME_SPRITES spriteID);
void drawGameSprite(SDL_Renderer*& renderer, const SPRITE_DATA& sprite, const VECTOR2i& offset, const std::array<SDL2Addon::SDL_COLOR, 2>& palette, const bool isPressed = false);