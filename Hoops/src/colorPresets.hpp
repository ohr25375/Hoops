#pragma once

#include <array>
#include <string>
#include "src/SDL2_Addon.hpp"

std::array<SDL2Addon::SDL_COLOR,2> getColorPreset(const int& index);
std::string getColorPresetName(const int& index);
size_t getPresetSize();