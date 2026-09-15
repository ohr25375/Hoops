#pragma once

#include <vector>
#include <cstdint>

#include "image.hpp"

namespace RIFFReader {
    void decodeRIFF(std::vector<uint8_t>& rawFileData);
    void decode(std::vector<uint8_t>& data, int depth = 0);
    void decodeHeader(std::vector<uint8_t>& data, int depth = 0);
    void decodeChunk(std::vector<uint8_t>& data, int depth = 0);
    image::animatedImage getRIFFImages();
    
    void decodeAnih(std::vector<uint8_t> data, int depth = 0);
    void decodeIcon(std::vector<uint8_t> data, int depth = 0);
    void decodeSeq(std::vector<uint8_t> data, int depth = 0);
    std::vector<uint8_t> getPayload(std::vector<uint8_t>::iterator& it, int length, bool advance = true);
}
