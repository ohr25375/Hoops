#pragma once

#include <cstdint>
#include <array>
#include <SDL.h>
#include "src/vector2.hpp"
#include "src/SDL2Util_ColorWrapper.hpp"

class HOOPS {
public:
    HOOPS(const std::array<uint16_t, 3> data = {}, const bool isSymmetric = false);
    void rotateRight();
    void rotateLeft();
    void rotateDepth();
    void setRandomRotaionDepth();
    void render(SDL_Renderer*& renderer, const VECTOR2i& offset, const std::array<SDL2Util::Color, 2>& palette, const bool isGhost = false) const;
    bool isHoopExist(const int x, const int y, const int layer) const;
    uint16_t getOrientation() const;
    bool getIsSymmetric() const;
private:
    std::array<uint16_t, 3> data;
    uint16_t orientation = 0;
    bool isSymmetric = false;
};