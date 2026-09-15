#include "CUR.hpp"

#include <iostream>

#include "hexManipulator.hpp"

namespace CUR {
    FTYPE::WORD CUR_ENTRIES::getXHotspot() {
        return var1;
    }
    FTYPE::WORD CUR_ENTRIES::getYHotspot() {
        return var2;
    }
    void CUR_ENTRIES::print() {
        std::cout << "Width         : " << (uint32_t)width << '\n';
        std::cout << "Height        : " << (uint32_t)height << '\n';
        std::cout << "Color Count   : " << (uint32_t)colorCount << '\n';
        std::cout << "Reserved      : " << (uint32_t)reserved << '\n';
        std::cout << "X Hotspot     : " << getXHotspot() << '\n';
        std::cout << "Y Hotspot     : " << getYHotspot() << '\n';
        std::cout << "Size In Bytes : " << sizeInBytes << '\n';
        std::cout << "File Offset   : " << fileOffset << '\n';
    }
    CUR_ENTRIES::CUR_ENTRIES() : ENTRIES() {}
    CUR_ENTRIES::CUR_ENTRIES(std::vector<FTYPE::BYTE>::iterator& it) : ENTRIES(it) {}

    CUR::CUR() {
        reserved = 0;
        type = 2;
        count = 0;
        iconInfos = std::vector<CUR_ENTRIES>(count);
        infoHeader = BMP::INFO_HEADER();
        colorData = std::vector<FTYPE::BYTE>(infoHeader.imageSize);
    }
    CUR::CUR(std::vector<FTYPE::BYTE>::iterator &it) {
        reserved    = hexManipulator::getValue<FTYPE::WORD >(it);
        type        = hexManipulator::getValue<FTYPE::WORD >(it);
        count       = hexManipulator::getValue<FTYPE::WORD >(it);
        iconInfos = std::vector<CUR_ENTRIES>(count);
        for (uint32_t i = 0; i < count; i++) {
            iconInfos[i] = CUR_ENTRIES(it);
        }
        infoHeader = BMP::INFO_HEADER(it);
        colorData = std::vector<FTYPE::BYTE>(it, it + infoHeader.imageSize);
        it += infoHeader.imageSize;
    }
    std::vector<COLOR::COLOR> CUR::getColor(bool debug) const {
        return ICO::getColorData(infoHeader, colorData, debug);
    }
    void CUR::print() {
        std::cout << "Reserved    : " << reserved << '\n';
        std::cout << "Type        : " << type << '\n';
        std::cout << "Entry Count : " << count << '\n';
        for (uint32_t i = 0; i < count; i++) {
            iconInfos[i].print();
        }
        infoHeader.print();
    }
} // CUR