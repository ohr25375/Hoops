#pragma once
#ifndef ANI_TO_IMAGE
#define ANI_TO_IMAGE

#include <vector>

#include "image.hpp"
#include "ANI.hpp"


namespace ANIToImage {
    image::animatedImage convert(const ANI::ANI& ani, bool debug = false);
    std::vector<FTYPE::BYTE> toData(const image::animatedImage& animatedImage);
};

#endif