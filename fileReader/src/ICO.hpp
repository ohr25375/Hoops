#pragma once
#ifndef ICO_FILE
#define ICO_FILE

#include <vector>

#include "filesystemTypes.hpp"
#include "BMP.hpp"
#include "color.hpp"


namespace ICO {
    class ENTRIES {
    public:
        FTYPE::BYTE width;
        FTYPE::BYTE height;
        FTYPE::BYTE colorCount;
    protected:
        FTYPE::BYTE reserved;
        FTYPE::WORD var1;
        FTYPE::WORD var2;
    public:
        FTYPE::DWORD sizeInBytes;
        FTYPE::DWORD fileOffset;

        ENTRIES();
        ENTRIES(std::vector<FTYPE::BYTE>::iterator& it);
    };

    class ICO_ENTRIES : public ENTRIES {
    public:
        FTYPE::WORD getplaneCount();
        FTYPE::WORD getbitCount();

        ICO_ENTRIES();
        ICO_ENTRIES(std::vector<FTYPE::BYTE>::iterator& it);
        void print();
    };

    class ICO {
    private:
        FTYPE::WORD reserved;
    public:
        FTYPE::WORD type;
        FTYPE::WORD count;
        std::vector<ICO_ENTRIES> iconInfos;
        BMP::INFO_HEADER infoHeader;
    private:
        std::vector<FTYPE::BYTE> colorData;
    public:
        ICO();
        ICO(std::vector<FTYPE::BYTE>::iterator& it);

        std::vector<COLOR::COLOR> getColor() const;
        void print();
    };

    std::vector<COLOR::COLOR> getColorData(const BMP::INFO_HEADER& infoHeader, const std::vector<FTYPE::BYTE>& colorData, bool debug = false);
} // ICO

#endif