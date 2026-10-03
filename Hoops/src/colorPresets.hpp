#pragma once

#include <array>
#include <string>
#include "src/SDL2Util_ColorWrapper.hpp"

std::array<SDL2Util::Color,2> getColorPreset(const int& index);
std::string getColorPresetName(const int& index);
size_t getPresetSize();