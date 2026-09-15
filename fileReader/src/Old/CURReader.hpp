#pragma once

#include <vector>
#include <cstdint>
#include "stringOperation.hpp"
#include "color.hpp"
#include "image.hpp"

namespace CURReader {
    class CURPalette {
    public:
        uint8_t b,g,r,a;
        CURPalette(uint32_t brg0 = 0);
        operator color::color() const;
    };

    class CUREntry {
    public:
        uint8_t width, height;
        uint8_t colorCount;
        uint16_t xHotspot;
        uint16_t yHotspot;
        uint32_t sizeOfData;
        uint32_t headerLocation;
        std::vector<uint8_t> getData();
        void printData(PRINTER pp);
    };
    
    class CUR {
    public:
        uint16_t imageCount;
        std::vector<CUREntry> entries;
        void entriesInit();
        uint32_t width, height;
        uint16_t planes;
        uint16_t bitsPerPixel;
        uint32_t compression;
        void setImageDataByteCount(uint32_t val);
        uint32_t getImageDataByteCount();
        std::vector<CURPalette> palette;
        void paletteInit();
        std::vector<uint8_t> colorData;
        void colorDataInit();
        std::vector<uint8_t> maskData;
        void maskInit();

        std::vector<uint8_t> getData();
        void printData(PRINTER pp);
        std::vector<color::color> getPixelColors();
        image::image getUniversalImageData();
        void printMask(bool flipY);
    private:
        uint32_t dataByteCount;
        uint32_t rowSize;
        uint32_t maskRowSize;
        bool isColorIndexed;
    };
    
    CUR decodeCUR(std::vector<uint8_t>& data, int depth = 0);
}
