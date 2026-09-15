#pragma once

#include <vector>
#include <array>
#include "src/SDL2_Addon.hpp"
#include "src/vector2.hpp"
#include <SDL2/SDL.h>
#include "hoops.hpp"

struct FALLING_RING {
    char data{};
    float currentHeight{};
    float targetHeight{};
};

struct FALLING_COLUMN {
    std::vector<FALLING_RING> fallingRings{};
};

struct FALLING_RINGS {
    std::vector<FALLING_COLUMN> fallingColumns{}; 
};

class GAME_BOARD {
public:
    GAME_BOARD(const int x, const int y);
    GAME_BOARD();

    void clear();
    int getPitch() const;
    int getHeight() const;
    char getCell(const int x, const int y) const;
    void setCell(const int x, const int y, const char state);
    void dropColumn(const int x, const int y);
    FALLING_COLUMN getDropHeights(const int x, const int heightThreshold) const;
    void render(SDL_Renderer*& renderer, const std::array<SDL2Addon::SDL_COLOR, 2>& palette, const VECTOR2i& offset) const;
    bool isColliding(const int x, const int y, const HOOPS& hoops) const;
    bool addHoops(const int x, const int y, const HOOPS& hoops);

    std::vector<std::vector<int>> getIslands(const int threshold = 0) const;
private:
    std::vector<char> board;
    int pitch;

    size_t getIndex(const int x, const int y) const;
};