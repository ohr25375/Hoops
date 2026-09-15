#include "ANItoImage.hpp"

#include <cstdint>
#include <iostream>

namespace ANIToImage {
    image::animatedImage convert(const ANI::ANI &ani, bool debug) {
        image::animatedImage res;
        res.frames = std::vector<image::image>(ani.header.frameCount);
        if (debug) std::cout << "Found " << ani.header.frameCount << " frames\n";
        for (uint32_t i = 0; i < ani.header.frameCount; i++) {
            if (debug) std::cout << "Setting width : " << ani.frames.icons[i].cursorData.infoHeader.width << "...\n";
            res.frames[i].width = ani.frames.icons[i].cursorData.infoHeader.width;
            if (debug) std::cout << "Setting height : " << ani.frames.icons[i].cursorData.infoHeader.height / 2 << "...\n";
            res.frames[i].height = ani.frames.icons[i].cursorData.infoHeader.height / 2;
            if (debug) std::cout << "Transferring colors...\n";
            res.frames[i].pixelData = ani.frames.icons[i].cursorData.getColor(debug);
        }
        if (debug) std::cout << "Setting frameOrder \n";
        res.frameOrder = ani.sequence.frameOrder;
        if (debug) std::cout << "Setting rate \n";
        res.rate = ani.rate.timing;
        return res;
    }

    std::vector<FTYPE::BYTE> toData(const image::animatedImage &animatedImage) {
        return animatedImage.toRBIC();
    }
}