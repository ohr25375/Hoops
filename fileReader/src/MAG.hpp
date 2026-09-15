#pragma once
#include "filesystemTypes.hpp"
#include <vector>

#include "color.hpp"

namespace MAG {
    struct CHECKSUM {
        std::vector<FTYPE::BYTE> mode = {'M', 'A', 'K', 'I', '0', '2', ' ', ' '};
        std::vector<FTYPE::BYTE> machine = {'M', 'P', 'S', ' '};
        std::vector<FTYPE::BYTE> user = {' ', '>', 0x93, 0xe4, '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
        std::vector<FTYPE::BYTE> memo = {' ', 0x1a};
    };
    CHECKSUM fillChecksum(const uint8_t* data);

    struct HEADER {
        FTYPE::BYTE top;
        FTYPE::BYTE machine;
        FTYPE::BYTE flags;
        FTYPE::BYTE mode;
        FTYPE::WORD xStart;
        FTYPE::WORD yStart;
        FTYPE::WORD xEnd;
        FTYPE::WORD yEnd;
        FTYPE::DWORD flagAOffset;
        FTYPE::DWORD flagBOffset;
        FTYPE::DWORD flagBSize;
        FTYPE::DWORD pixelOffset;
        FTYPE::DWORD pixelSize;
    };
    HEADER fillHeader(const uint8_t* data);

    struct PALETTE {
        std::vector<COLOR::COLOR> colors;
    };
    PALETTE fillPalette(const uint8_t* data, const int colorCount);

    struct FLAGAB {
        std::vector<uint8_t> flagA;
        std::vector<uint8_t> flagB;
    };
    FLAGAB fillFlagAB(const uint8_t* data, const size_t flagASize, const size_t flagBSize);

    struct PIXEL_DATA {
        std::vector<uint8_t> data;
    };
    PIXEL_DATA fillPixelData(const uint8_t* data, const size_t size);

    struct MAG {
        CHECKSUM checksum;
        HEADER header;
        PALETTE palette;
        FLAGAB flagAB;
        PIXEL_DATA pixelData;
    };

    class LOADER {
    public:
        void printMAG(const MAG& mag) const;
        MAG loadMAG(const uint8_t* data) const;
        std::vector<COLOR::COLOR> extractPixelData(const MAG& mag) const;
        std::vector<uint8_t> extractIndexedData(const MAG& mag) const;
        std::vector<uint8_t> createData16(const std::vector<uint8_t>& indexedData, const uint16_t pitch, const std::vector<COLOR::COLOR>& palette, const CHECKSUM& checksum = CHECKSUM()) const;
    private:
        std::vector<uint8_t> compressNibbles(const std::vector<uint8_t>& nibbles) const;
        int fillFlagAndPixelBuffer(const std::vector<uint8_t>& pixelData, std::vector<uint8_t>& flagBuffer, std::vector<uint8_t>& pixelBuffer, const uint16_t pitch) const;
        int formatFlagBuffer(std::vector<uint8_t>& flagBuffer, const uint16_t height) const;
        int fillFlagAB(const std::vector<uint8_t>& flagBuffer, std::vector<uint8_t>& flagA, std::vector<uint8_t>& flagB) const;
        std::vector<uint8_t> expandPalette(const std::vector<COLOR::COLOR>& palette) const;
        std::vector<uint8_t> createDataStream(const std::vector<uint8_t>& flagA, const std::vector<uint8_t>& flagB, const std::vector<uint8_t>& pixelBuffer, const std::vector<uint8_t>& paletteData, const CHECKSUM& checksum = {}) const;
        std::vector<uint8_t> _extractIndexedData(const MAG& mag) const;
        std::vector<uint8_t> _correct200LineIndexedData(const std::vector<uint8_t>& data, const int pitch) const;

    };
};