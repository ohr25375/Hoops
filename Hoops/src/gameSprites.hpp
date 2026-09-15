#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include <cstdint>
#include <array>
#include "src/vector2.hpp"
#include "src/SDL2_Addon.hpp"

struct SPRITE_ROW {
    uint8_t color;
    uint8_t alpha;
};

typedef std::array<SPRITE_ROW, 8> SPRITE;

enum GAME_SPRITES {
    GAME_SPRITE_NONE,
    GAME_SPRITE_BLANK,
    GAME_SPRITE_CURSOR,
    GAME_SPRITE_HOOP_001,
    GAME_SPRITE_HOOP_010,
    GAME_SPRITE_HOOP_100,
    GAME_SPRITE_CONNECTOR_RIGHT,
    GAME_SPRITE_CONNECTOR_DOWN,
    GAME_SPRITE_CONNECTOR_LEFT,
    GAME_SPRITE_CONNECTOR_UP,
    GAME_SPRITE_HOOP_001_GHOST,
    GAME_SPRITE_HOOP_010_GHOST,
    GAME_SPRITE_HOOP_100_GHOST,
    GAME_SPRITE_LEFT_WALL,
};

struct SPRITE_DATA {
    std::vector<SDL_Point> whites{};
    std::vector<SDL_Point> blacks{};
};

SPRITE_DATA getGameSprite(const GAME_SPRITES spriteID);
void drawGameSprite(SDL_Renderer*& renderer, const SPRITE_DATA& sprite, const VECTOR2i& offset, const std::array<SDL2Addon::SDL_COLOR, 2>& palette, const bool isPressed);