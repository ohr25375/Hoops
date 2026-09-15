#include "ICO.hpp"

#include "hexManipulator.hpp"

namespace ICO {
    bool isDebug = false;
    ENTRIES::ENTRIES() {}
    ENTRIES::ENTRIES(std::vector<FTYPE::BYTE>::iterator &it) {
        width       = hexManipulator::getValue<FTYPE::BYTE >(it);
        height      = hexManipulator::getValue<FTYPE::BYTE >(it);
        colorCount  = hexManipulator::getValue<FTYPE::BYTE >(it);
        reserved    = hexManipulator::getValue<FTYPE::BYTE >(it);
        var1        = hexManipulator::getValue<FTYPE::WORD >(it);
        var2        = hexManipulator::getValue<FTYPE::WORD >(it);
        sizeInBytes = hexManipulator::getValue<FTYPE::DWORD>(it);
        fileOffset  = hexManipulator::getValue<FTYPE::DWORD>(it);
    }
    
    FTYPE::WORD ICO_ENTRIES::getplaneCount() {
        return var1;
    }
    FTYPE::WORD ICO_ENTRIES::getbitCount() {
        return var2;
    }
    void ICO_ENTRIES::print() {
        std::cout << "Width         : " << width << '\n';
        std::cout << "Height        : " << height << '\n';
        std::cout << "Color Count   : " << colorCount << '\n';
        std::cout << "Reserved      : " << reserved << '\n';
        std::cout << "Plane Count   : " << getplaneCount() << '\n';
        std::cout << "Bit Count     : " << getbitCount() << '\n';
        std::cout << "Size In Bytes : " << sizeInBytes << '\n';
        std::cout << "File Offset   : " << fileOffset << '\n';
    }
    ICO_ENTRIES::ICO_ENTRIES() : ENTRIES() {}
    ICO_ENTRIES::ICO_ENTRIES(std::vector<FTYPE::BYTE>::iterator& it) : ENTRIES(it) {}

    ICO::ICO() {
        reserved = 0;
        type = 2;
        count = 0;
        iconInfos = std::vector<ICO_ENTRIES>(count);
        infoHeader = BMP::INFO_HEADER();
        colorData = std::vector<FTYPE::BYTE>(infoHeader.imageSize);
    }
    ICO::ICO(std::vector<FTYPE::BYTE>::iterator &it) {
        reserved    = hexManipulator::getValue<FTYPE::WORD >(it);
        type        = hexManipulator::getValue<FTYPE::WORD >(it);
        count       = hexManipulator::getValue<FTYPE::WORD >(it);
        iconInfos = std::vector<ICO_ENTRIES>(count);
        for (uint32_t i = 0; i < count; i++) {
            iconInfos[i] = ICO_ENTRIES(it);
        }
        infoHeader = BMP::INFO_HEADER(it);
        std::cout << "image size : " << infoHeader.imageSize << '\n';
        colorData = std::vector<FTYPE::BYTE>(it, it + infoHeader.imageSize);
        it += infoHeader.imageSize;
    }
    std::vector<COLOR::COLOR> getColorData(const BMP::INFO_HEADER& infoHeader, const std::vector<FTYPE::BYTE>& colorData, bool debug) {
        isDebug = debug;
        uint32_t height = infoHeader.height / 2;
        if (isDebug) std::cout << "Set dimension to : " << infoHeader.width * height << '\n';
        std::vector<COLOR::COLOR> res(infoHeader.width * height);

        uint32_t paletteSize = 0;
        if (isDebug) std::cout << "color depth is : " << infoHeader.colorDepth << '\n';
        if (infoHeader.colorDepth <= 8) {
            paletteSize = 1 << infoHeader.colorDepth;
        }
        if (isDebug) std::cout << "Set palette size to : " << paletteSize << '\n';
        std::vector<COLOR::COLOR> palette(paletteSize);
        for (uint32_t i = 0; i < paletteSize; i++) {
            palette[i].b = colorData[(i * 4) + 0];
            palette[i].g = colorData[(i * 4) + 1];
            palette[i].r = colorData[(i * 4) + 2];
            palette[i].a = colorData[(i * 4) + 3];

            palette[i].a = 0xff;
        }
        if (isDebug) std::cout << "Reading colors...\n";
        uint32_t bytesPerRow = ((infoHeader.colorDepth * infoHeader.width + 31) / 32) * 4;
        try {
        for (uint32_t y = 0; y < height; y++) {
            uint32_t rowStartIndex = paletteSize * 4 + y * bytesPerRow;
            for (uint32_t x = 0; x < infoHeader.width; x++) {
                uint32_t pixelIndex = x + (height - y - 1) * infoHeader.width;
                switch (infoHeader.colorDepth) {
                    case 1:
                    case 2:
                    case 4:
                    case 8: {
                        uint32_t dataInByte = 8 / infoHeader.colorDepth;
                        uint32_t mask = (1 << infoHeader.colorDepth) - 1;
                        uint32_t index = rowStartIndex + x;
                        for (uint32_t b = 1; b <= dataInByte; b++) {
                            uint32_t rshift = 8 - (b * infoHeader.colorDepth);
                            uint32_t paletteIndex = (colorData[index] >> rshift) & mask;
                            res[pixelIndex] = palette[paletteIndex];
                        }
                        break;
                    }
                    case 16: {
                        uint32_t index = rowStartIndex + x * 2;
                        FTYPE::WORD c = colorData[index] + colorData[index + 1] << 8;
                        res[pixelIndex].b = (c >> 0) & 0x3f;
                        res[pixelIndex].g = (c >> 5) & 0x3f;
                        res[pixelIndex].r = (c >> 10) & 0x3f;
                        break;
                    }
                    case 24: {
                        uint32_t index = rowStartIndex + x * 3;
                        res[pixelIndex].b = colorData[index + 0];
                        res[pixelIndex].g = colorData[index + 1];
                        res[pixelIndex].r = colorData[index + 2];
                        break;
                    }
                    case 32: {

                            uint32_t index = rowStartIndex + x * 4;
                            if (pixelIndex >= res.size()) {
                                throw std::runtime_error("pixel index out of bounds\n");
                            }
                            if (index + 3 >= colorData.size()) {
                                throw std::runtime_error("color data index out of bounds : " + std::to_string(index + 3) + "\n");
                            }
                            res[pixelIndex].b = colorData[index + 0];
                            res[pixelIndex].g = colorData[index + 1];
                            res[pixelIndex].r = colorData[index + 2];
                            res[pixelIndex].a = colorData[index + 3];
                        break;
                    }
                }
            }
        }
        } catch (std::exception& e) {
            std::cout << e.what() << '\n';
        }
        if (isDebug) std::cout << "Masking...\n";
        uint32_t maskStartIndex = paletteSize * 4 + height * bytesPerRow;
        bytesPerRow = ((infoHeader.width + 31) / 32) * 4;
        for (uint32_t y = 0; y < height; y++) {
            uint32_t rowStartIndex = maskStartIndex + y * bytesPerRow;
            for (uint32_t x = 0; x < infoHeader.width; x++) {
                uint32_t pixelIndex = x + (height - y - 1) * infoHeader.width;
                uint32_t index = rowStartIndex + x / 8;
                uint8_t maskByte = colorData[index];
                uint32_t rshift = 8 - (x % 8) - 1;
                uint32_t mask = (maskByte >> rshift) & 0b1;
                res[pixelIndex].a *= (-mask) + 1;
            }
        }
        if (isDebug) std::cout << "Finished converting to colors...\n";
        return res;
    }
    std::vector<COLOR::COLOR> ICO::getColor() const {
        return getColorData(infoHeader, colorData);
    }
    void ICO::print()
    {
        std::cout << "Reserved    : " << reserved << '\n';
        std::cout << "Type        : " << type << '\n';
        std::cout << "Entry Count : " << count << '\n';
        for (uint32_t i = 0; i < count; i++) {
            iconInfos[i].print();
        }
        infoHeader.print();
    }
} // ICO
