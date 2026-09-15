#pragma once

#include "image.hpp"
#include "RBIC.hpp"

namespace imageToRBICConverter {
    const uint32_t RBIC_VERSION = 0;

    RBIC::RBIC convertImageToRBIC(const image::image& image);
    RBIC::RBIC convertAnimatedImageToRBIC(const image::animatedImage& animatedImage);
}