#include "BMP.hpp"

#include <iostream>

#include "hexManipulator.hpp"

namespace BMP {
    INFO_HEADER::INFO_HEADER() {
        headerSize = 40;
        width = 0;
        height = 0;
        planes = 0;
        colorDepth = 0;
        compression = 0;
        imageSize = 0;
    }

    INFO_HEADER::INFO_HEADER(std::vector<FTYPE::BYTE>::iterator &it) {
        headerSize = hexManipulator::getValue<FTYPE::DWORD>(it);
        width = hexManipulator::getValue<FTYPE::DWORD>(it);
        height = hexManipulator::getValue<FTYPE::DWORD>(it);
        planes = hexManipulator::getValue<FTYPE::WORD>(it);
        colorDepth = hexManipulator::getValue<FTYPE::WORD>(it);
        compression = hexManipulator::getValue<FTYPE::DWORD>(it);
        imageSize = hexManipulator::getValue<FTYPE::DWORD>(it);
        xPixelPerM = hexManipulator::getValue<FTYPE::DWORD>(it);
        yPixelPerM = hexManipulator::getValue<FTYPE::DWORD>(it);
        colorsUsed = hexManipulator::getValue<FTYPE::DWORD>(it);
        colorsImportant = hexManipulator::getValue<FTYPE::DWORD>(it);
        if (imageSize == 0) {
            uint32_t pixelCount = width * (height / 2);
            uint32_t maskSize = pixelCount / 8;
            uint32_t imageBytes = pixelCount * (colorDepth / 8.0);
            imageSize = imageBytes + maskSize;
            if (colorDepth > 8) return;
            uint32_t paletteSize = (1 << colorDepth) * 4;
            imageSize += paletteSize;
        }
    }

    void INFO_HEADER::print() {
        std::cout << "Header Size : " << headerSize << '\n';
        std::cout << "Width       : " << width << '\n';
        std::cout << "Height      : " << height << '\n';
        std::cout << "Planes      : " << planes << '\n';
        std::cout << "Color Depth : " << colorDepth << '\n';
        std::cout << "Compression : " << compression << '\n';
        std::cout << "Image Size  : " << imageSize << '\n';
    }
} // BMP
