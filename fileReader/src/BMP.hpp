#pragma once
#ifndef BMP_FILE
#define BMP_FILE

#include <vector>

#include "filesystemTypes.hpp"

namespace BMP {
    class INFO_HEADER {
    public:
        FTYPE::DWORD headerSize;
        FTYPE::DWORD width;
        FTYPE::DWORD height;
        FTYPE::WORD planes;
        FTYPE::WORD colorDepth;
        FTYPE::DWORD compression;
        FTYPE::DWORD imageSize;
        FTYPE::DWORD xPixelPerM;
        FTYPE::DWORD yPixelPerM;
        FTYPE::DWORD colorsUsed;
        FTYPE::DWORD colorsImportant;

        INFO_HEADER();
        INFO_HEADER(std::vector<FTYPE::BYTE>::iterator& it);

        void print();
    };
} // BMP

#endif