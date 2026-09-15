#include "gameSprites.hpp"
#include <iostream>
#include <array>

#include "src/SDL2_Addon.hpp"
// #include "canvasManipulation.hpp"

const std::vector<SPRITE> gameSprites {
    { // none
        SPRITE_ROW{},
        SPRITE_ROW{},
        SPRITE_ROW{},
        SPRITE_ROW{},
        SPRITE_ROW{},
        SPRITE_ROW{},
        SPRITE_ROW{},
        SPRITE_ROW{},
    },
    { // blank
        SPRITE_ROW{.alpha = 0b11111111},
        SPRITE_ROW{.alpha = 0b11111111},
        SPRITE_ROW{.alpha = 0b11111111},
        SPRITE_ROW{.alpha = 0b11111111},
        SPRITE_ROW{.alpha = 0b11111111},
        SPRITE_ROW{.alpha = 0b11111111},
        SPRITE_ROW{.alpha = 0b11111111},
        SPRITE_ROW{.alpha = 0b11111111},
    },
    { // cursor
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b11111111},
        SPRITE_ROW{.color = 0b01111110, .alpha = 0b11111111},
        SPRITE_ROW{.color = 0b01111100, .alpha = 0b11111110},
        SPRITE_ROW{.color = 0b01111000, .alpha = 0b11111100},
        SPRITE_ROW{.color = 0b01110000, .alpha = 0b11111000},
        SPRITE_ROW{.color = 0b01100000, .alpha = 0b11110000},
        SPRITE_ROW{.color = 0b01000000, .alpha = 0b11100000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b11000000},
    },
    { // block 001
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b00111000, .alpha = 0b00111000},
        SPRITE_ROW{.color = 0b00101000, .alpha = 0b00101000},
        SPRITE_ROW{.color = 0b00111000, .alpha = 0b00111000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
    },
    { // block 010
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b01111100, .alpha = 0b01111100},
        SPRITE_ROW{.color = 0b01000100, .alpha = 0b01000100},
        SPRITE_ROW{.color = 0b01000100, .alpha = 0b01000100},
        SPRITE_ROW{.color = 0b01000100, .alpha = 0b01000100},
        SPRITE_ROW{.color = 0b01111100, .alpha = 0b01111100},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
    },
    { // block 100
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b11111110, .alpha = 0b11111110},
        SPRITE_ROW{.color = 0b10000010, .alpha = 0b10000010},
        SPRITE_ROW{.color = 0b10000010, .alpha = 0b10000010},
        SPRITE_ROW{.color = 0b10000010, .alpha = 0b10000010},
        SPRITE_ROW{.color = 0b10000010, .alpha = 0b10000010},
        SPRITE_ROW{.color = 0b10000010, .alpha = 0b10000010},
        SPRITE_ROW{.color = 0b11111110, .alpha = 0b11111110},
    },
    { // right connector
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b00000001, .alpha = 0b00000001},
        SPRITE_ROW{.color = 0b00000001, .alpha = 0b00000001},
        SPRITE_ROW{.color = 0b00000001, .alpha = 0b00000001},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00001111},
        SPRITE_ROW{.color = 0b00000001, .alpha = 0b00000001},
        SPRITE_ROW{.color = 0b00000001, .alpha = 0b00000001},
        SPRITE_ROW{.color = 0b00000001, .alpha = 0b00000001},
    },
    { // down connector
        SPRITE_ROW{.alpha = 0b00000000},
        SPRITE_ROW{.alpha = 0b00000000},
        SPRITE_ROW{.alpha = 0b00000000},
        SPRITE_ROW{.alpha = 0b00000000},
        SPRITE_ROW{.alpha = 0b00000000},
        SPRITE_ROW{.alpha = 0b00010000},
        SPRITE_ROW{.alpha = 0b00010000},
        SPRITE_ROW{.alpha = 0b00010000},
    },
    { // left connector
        SPRITE_ROW{.alpha = 0b00000000},
        SPRITE_ROW{.alpha = 0b00000000},
        SPRITE_ROW{.alpha = 0b00000000},
        SPRITE_ROW{.alpha = 0b00000000},
        SPRITE_ROW{.alpha = 0b11100000},
        SPRITE_ROW{.alpha = 0b00000000},
        SPRITE_ROW{.alpha = 0b00000000},
        SPRITE_ROW{.alpha = 0b00000000},
    },
    { // up connector
        SPRITE_ROW{.color = 0b11101110, .alpha = 0b11111110},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00010000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00010000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00010000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
    },
    { // block 001 GHOST
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b00101000, .alpha = 0b00111000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00101000},
        SPRITE_ROW{.color = 0b00101000, .alpha = 0b00111000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
    },
    { // block 010 GHOST
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b01010100, .alpha = 0b01111100},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b01000100},
        SPRITE_ROW{.color = 0b01000100, .alpha = 0b01000100},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b01000100},
        SPRITE_ROW{.color = 0b01010100, .alpha = 0b01111100},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
    },
    { // block 100 GHOST
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b00000000},
        SPRITE_ROW{.color = 0b10101010, .alpha = 0b11111110},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b10000010},
        SPRITE_ROW{.color = 0b10000010, .alpha = 0b10000010},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b10000010},
        SPRITE_ROW{.color = 0b10000010, .alpha = 0b10000010},
        SPRITE_ROW{.color = 0b00000000, .alpha = 0b10000010},
        SPRITE_ROW{.color = 0b10101010, .alpha = 0b11111110},
    },
    { // Lwall
        SPRITE_ROW{.color = 0b10000010, .alpha = 0b11111110},
        SPRITE_ROW{.color = 0b11111110, .alpha = 0b11111110},
        SPRITE_ROW{.color = 0b11000110, .alpha = 0b11111110},
        SPRITE_ROW{.color = 0b10101010, .alpha = 0b11111110},
        SPRITE_ROW{.color = 0b10010010, .alpha = 0b11111110},
        SPRITE_ROW{.color = 0b10101010, .alpha = 0b11111110},
        SPRITE_ROW{.color = 0b11000110, .alpha = 0b11111110},
        SPRITE_ROW{.color = 0b11111110, .alpha = 0b11111110},
    },
};

void generateFromSprite(const SPRITE& sprite, SPRITE_DATA& out) {
    int yMax = sprite.size();
    int xMax = sizeof(sprite[0].color) * __CHAR_BIT__;
    for (auto sy = 0; sy < yMax; sy++) {
        auto sr = sprite[sy];
        for (auto sx = 0; sx < xMax; sx++) {
            bool isAlpha = (sr.alpha >> (xMax - 1 - sx)) & 1;
            if (!isAlpha) continue;
            bool isColor = (sr.color >> (xMax - 1 - sx)) & 1;
            auto& color = isColor ? out.whites : out.blacks;
            color.push_back({.x = sx, .y = sy});
        }
    }
}

std::vector<SPRITE_DATA> generate() {
    auto res = std::vector<SPRITE_DATA>(gameSprites.size());
    for (auto i = 0; i < (int)res.size(); i++) {
        auto sprite = gameSprites[i];
        generateFromSprite(sprite, res[i]);
    }
    return res;
}

SPRITE_DATA getGameSprite(const GAME_SPRITES spriteID) {
    static const auto editorSprites = generate();
    if (spriteID < editorSprites.size()) return editorSprites[(int)spriteID];
    std::cout << "ERROR: Editor Sprite not found\n";
    return editorSprites[0];
}
void drawGameSprite(SDL_Renderer*& renderer, const SPRITE_DATA& sprite, const VECTOR2i& offset, const std::array<SDL2Addon::SDL_COLOR, 2>& palette, const bool invert) {
    auto offsetSprite = sprite;
    for (auto& point : offsetSprite.whites) {
        point = VECTOR2i(point.x, point.y) + offset;
    }
    for (auto& point : offsetSprite.blacks) {
        point = VECTOR2i(point.x, point.y) + offset;
    }
    SDL2Addon::DrawPoints(renderer, offsetSprite.whites, palette[invert ? 0 : 1]);
    SDL2Addon::DrawPoints(renderer, offsetSprite.blacks, palette[invert ? 1 : 0]);
}