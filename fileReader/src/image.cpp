#include "image.hpp"

#include <iostream>

#define endl '\n'

#define expand32(n) (uint8_t)(((n) >> 0) & 0xff),(uint8_t)(((n) >> 8) & 0xff),(uint8_t)(((n) >> 16) & 0xff),(uint8_t)(((n) >> 24) & 0xff)
#define all(n) n.begin(),n.end()

namespace image {
    image::image() {
        width = 0;
        height = 0;
        pixelData = std::vector<COLOR::COLOR>();
    }
    
    std::vector<uint8_t> image::toData() {
        uint32_t dataSize = pixelData.size() * 4;
        std::vector<uint8_t> data(8 + dataSize);
        data[0] = width;
        data[4] = height;
        for (int i = 0; i < dataSize; i += 4) {
            int ci = i / 4;
            data[8 + i + 0] = pixelData[ci].a;
            data[8 + i + 1] = pixelData[ci].b;
            data[8 + i + 2] = pixelData[ci].g;
            data[8 + i + 3] = pixelData[ci].r;
        }
        return data;
    }
    
    std::vector<uint8_t> batchData(std::vector<image> images) {
        std::vector<uint8_t> data(0);
        uint8_t imageCount = (uint8_t)images.size();
        data.push_back(imageCount);
        for (int i = 0; i < imageCount; i++) {
            std::vector<uint8_t> iData = images[i].toData();
            data.insert(data.end(), iData.begin(), iData.end());
        }
        return data;
    }
    
    animatedImage::animatedImage() {
        frameOrder = std::vector<uint32_t>();
        frames = std::vector<image>();
        rate = std::vector<uint32_t>();
    }

    std::vector<uint8_t> animatedImage::toRBIC() const {
        // Raw Bitmap Image Container
        // "RBIC" < must be at the start
        //  4byte dataWidth (whole data width in bytes - 4)
        // "ANIM" < must be next
        //  4byte version = 0
        //  4byte dataWidth = 8
        //  4byte frameCount
        //  4byte stepCount
        // "DATA"
        //  4byte dataWidth
        //  "seq " < only once
        //   4byte dataWidth
        //   4 * stepCount byte frameOrder
        //  "bimg"
        //   4byte dataWidth
        //   4byte width
        //   4byte height
        //   4 * width * height byte pixelColorData

        uint32_t frameCount = frames.size();

        std::vector<uint8_t> tempData(0);
        writeSeq(tempData);
        writeRate(tempData);
        for (int i = 0; i < frameCount; i++) {
            writeBimg(tempData, frames[i]);
        }

        uint32_t tempDataWidth = (uint32_t)tempData.size();

        std::vector<uint8_t> data = {
            'R','B','I','C',
            0,0,0,0,
            'A','N','I','M',
            12,0,0,0,
            0,0,0,0,
            expand32(frameCount),
            expand32((uint32_t)frameOrder.size()),
            'D','A','T','A',
            expand32(tempDataWidth)
        };

        data.insert(data.end(), all(tempData));

        uint32_t dataWidth = (uint32_t)data.size() - 4;
        write32(data.begin() + 4, dataWidth);
        
        return data;
    }

    void printAlphaMap(animatedImage image) {
        for (int f = 0; f < image.frames.size(); f++) {
            for (int i = 0; i < image.frames[f].pixelData.size(); i++) {
                int a = image.frames[f].pixelData[i].a;
                std::cout << (a == 0 ? '0' : '1') << (i % image.frames[f].width == image.frames[f].width - 1 ? "\n" : "");
            }
            std::cout << endl;
        }
    }

    void animatedImage::writeSeq(std::vector<uint8_t>& data) const {
        //"seq " < only once
        // 4byte dataWidth
        // 4 * stepCount byte frameOrder

        std::vector<uint8_t> seq = {'s', 'e', 'q', ' '};

        int seqCount = (int)frameOrder.size();
        uint32_t seqDataWidth = seqCount * 4;
        std::vector<uint8_t> seqDataSize = {expand32(seqDataWidth)};
        seq.insert(seq.end(), all(seqDataSize));

        std::vector<uint8_t> seqData(seqDataWidth);
        for (int i = 0; i < seqCount; i++)
        {
            write32(seqData.begin() + i * 4, frameOrder[i]);
        }
        seq.insert(seq.end(), all(seqData));

        data.insert(data.end(), all(seq));
    }

    void animatedImage::writeRate(std::vector<uint8_t>& data) const {
        //"rate" < only once
        // 4byte dataWidth
        // 4 * stepCount byte frameOrder
        std::vector<uint8_t> res = {'r', 'a', 't', 'e'};
        int rateCount = (int)rate.size();

        uint32_t rateDataWidth = rateCount * 4;
        std::vector<uint8_t> rateDataSize = {expand32(rateDataWidth)};
        res.insert(res.end(), all(rateDataSize));

        std::vector<uint8_t> rateData(rateDataWidth);
        for (int i = 0; i < rateCount; i++)
        {
            write32(rateData.begin() + i * 4, rate[i]);
        }
        res.insert(res.end(), all(rateData));

        data.insert(data.end(), all(res));
    }

    void animatedImage::writeBimg(std::vector<uint8_t> &data, image imageObject) const {
        //  "bimg"
        //   4byte dataWidth
        //   4byte width
        //   4byte height
        //   4 * width * height byte pixelColorData
        std::vector<uint8_t> imageData = imageObject.toData();
        uint32_t imageDataWidth = (uint32_t)imageData.size();

        std::vector<uint8_t> bimg = {
            'b','i','m','g',
            expand32(imageDataWidth)
        };

        bimg.insert(bimg.end(), all(imageData));

        data.insert(data.end(), all(bimg));
    }

    void write32(std::vector<uint8_t>::iterator vector, uint32_t val) {
        for (int j = 0; j < 4; j++) {
            int rs = j * 8;
            *(vector + j) = (val >> rs) & 0xff;
        }
    }
}
