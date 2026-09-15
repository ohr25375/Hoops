#pragma once

#include <vector>
#include <cstdint>

#include "color.hpp"

namespace image {
    class image {
    public:
        uint8_t width, height;
        std::vector<COLOR::COLOR> pixelData;

        image();
        
        std::vector<uint8_t> toData();
    };

    class animatedImage {
    public:
        std::vector<uint32_t> frameOrder;
        std::vector<image> frames;
        std::vector<uint32_t> rate;

        animatedImage();

        std::vector<uint8_t> toRBIC() const;
    private:
        void writeSeq(std::vector<uint8_t>& data) const;
        void writeRate(std::vector<uint8_t>& data) const;
        void writeBimg(std::vector<uint8_t>& data, image imageObject) const;
    };
    
    void printAlphaMap(animatedImage image);
    void write32(std::vector<uint8_t>::iterator vector, uint32_t val);
    std::vector<uint8_t> batchData(std::vector<image> images);
}