#include "RBIC.hpp"

#include <iostream>
#include "hexManip.hpp"

#define endl '\n'

namespace RBIC {
    FILETYPE::FILETYPE(std::array<uint8_t, 4> header) : header(header) {}
    ANIM::ANIM() : FILETYPE({'A','N','I','M'}) {}
    IMAG::IMAG() : FILETYPE({'I','M','A','G'}) {}
    
    CHUNK::CHUNK(std::array<uint8_t, 4> header) : header(header) {}
    DATA::DATA() : CHUNK({'D','A','T','A'}) {}
    
    DATACHUNK::DATACHUNK(std::array<uint8_t, 4> header) : header(header) {}
    BIMG::BIMG() : DATACHUNK({'b','i','m','g'}) {}
    SEQ_::SEQ_() : DATACHUNK({'s', 'e', 'q', ' '}) {}
    RATE::RATE() : DATACHUNK({'r', 'a', 't', 'e'}) {}

    void recalculateDataSize(RBIC &rbic) {
        recalculateDataSize(rbic.fileType);
    }

    void recalculateDataSize(std::shared_ptr<FILETYPE> filetype) {
    }

    void recalculateDataSize(std::shared_ptr<BIMG> bimg) {
        bimg->dataSize = 8 + (bimg->width * bimg->height) * 4;
    }

    UNKNOWN_FILETYPE::UNKNOWN_FILETYPE(std::array<uint8_t, 4> header) : FILETYPE(header){}

    uint32_t headerToID(std::array<uint8_t, 4> header) {
        return hexManip::toUint32_t(header);
    }

    std::array<uint8_t, 4> idToHeader(uint32_t id) {
        return {
            (uint8_t)((id >>  0) & 0xff),
            (uint8_t)((id >>  8) & 0xff),
            (uint8_t)((id >> 16) & 0xff),
            (uint8_t)((id >> 24) & 0xff),
        };
    }
}