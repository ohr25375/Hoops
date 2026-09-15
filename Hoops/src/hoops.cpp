#include "hoops.hpp"
#include "gameSprites.hpp"

#include <iostream>

const int SPRITE_SIZE = 8;
const int LAYER_COUNT = 3;
const int HOOP_SIZE = 3;
const int DATA_SIZE = HOOP_SIZE * HOOP_SIZE;

int getDataIndex(const int x, const int y) {
    return x + y * HOOP_SIZE;
}

HOOPS::HOOPS(const std::array<uint16_t,3> data, const bool isSymmetric) {
    this->data = data;
    this->isSymmetric = isSymmetric;
}

uint16_t rotateClockwise(const uint16_t data) {
    uint8_t data1 = (data >> 6) & 0x7;
    uint8_t data2 = (data >> 3) & 0x7;
    uint8_t data3 = (data >> 0) & 0x7;

    uint8_t result1 =
        ((data3 & 0b100) >> 0) |
        ((data2 & 0b100) >> 1) |
        ((data1 & 0b100) >> 2);
    uint8_t result2 =
        ((data3 & 0b010) << 1) |
        ((data2 & 0b010) >> 0) |
        ((data1 & 0b010) >> 1);
    uint8_t result3 =
        ((data3 & 0b001) << 2) |
        ((data2 & 0b001) << 1) |
        ((data1 & 0b001) << 0);

    auto result = 
        (((uint16_t)result1) << 6) |
        (((uint16_t)result2) << 3) |
        (((uint16_t)result3) << 0);
    return result;
}

void HOOPS::rotateLeft() {
    auto res = this->data;
    for (auto& layer : res) {
        layer = rotateClockwise(rotateClockwise(rotateClockwise(layer)));
    }
    this->data = res;
    this->orientation = (this->orientation + 3) & 3;
}

void HOOPS::rotateDepth() {
    auto res = this->data;
    for (auto i = 0; i < 3; i++) {
        res[i] = this->data[(i + 1) % 3];
    }
    this->data = res;
}

void HOOPS::setRandomRotaionDepth() {
    auto random = SDL_GetTicks64();
    for (auto i = 0; i < (int)(random & 0b11); i++) {
        this->rotateLeft();
    }
    for (auto i = 0; i < (int)((random >> 2) % 3); i++) {
        this->rotateDepth();
    }
}

void HOOPS::rotateRight() {
    auto res = this->data;
    for (auto& layer : res) {
        layer = rotateClockwise(layer);
    }
    this->data = res;
    this->orientation = (this->orientation + 1) & 3;
}

void HOOPS::render(SDL_Renderer*& renderer, const VECTOR2i& offset, const std::array<SDL2Addon::SDL_COLOR, 2>& palette, const bool isGhost) const {
    auto baseSprite = isGhost ? GAME_SPRITE_HOOP_001_GHOST : GAME_SPRITE_HOOP_001;
    for (auto s = 0; s < LAYER_COUNT; s++) {
        auto layer = this->data[s];
        auto sprite = getGameSprite((GAME_SPRITES)(baseSprite + s));
        for (auto i = 0; i < DATA_SIZE; i++) {
            bool isHoop = (layer & (1 << (DATA_SIZE - 1 - i))) != 0;
            if (!isHoop) continue;
            auto cellOffset = VECTOR2i(i % HOOP_SIZE, i / HOOP_SIZE) * SPRITE_SIZE;
            drawGameSprite(renderer, sprite, cellOffset + offset, palette, false);
        }
    }
}



bool HOOPS::isHoopExist(const int x, const int y, const int layer) const {
    // 012
    // 345
    // 678
    return (data[layer] & (1 << (DATA_SIZE - 1 - getDataIndex(x, y)))) != 0;
}

uint16_t HOOPS::getOrientation() const {
    return this->orientation;
}

bool HOOPS::getIsSymmetric() const {
    return this->isSymmetric;
}
