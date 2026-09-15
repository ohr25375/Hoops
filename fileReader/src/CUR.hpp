#pragma once
#ifndef CUR_FILE
#define CUR_FILE

#include <vector>

#include "filesystemTypes.hpp"
#include "BMP.hpp"
#include "ICO.hpp"

namespace CUR {
    class CUR_ENTRIES : public ICO::ENTRIES {
    public:
        FTYPE::WORD getXHotspot();
        FTYPE::WORD getYHotspot();

        CUR_ENTRIES();
        CUR_ENTRIES(std::vector<FTYPE::BYTE>::iterator& it);
        void print();
    };

    class CUR {
    private:
        FTYPE::WORD reserved;
    public:
        FTYPE::WORD type;
        FTYPE::WORD count;
        std::vector<CUR_ENTRIES> iconInfos;
        BMP::INFO_HEADER infoHeader;
    private:
        std::vector<FTYPE::BYTE> colorData;
    public:
        CUR();
        CUR(std::vector<FTYPE::BYTE>::iterator& it);

        std::vector<COLOR::COLOR> getColor(bool debug = false) const;

        void print();
    };
} // CUR

#endif