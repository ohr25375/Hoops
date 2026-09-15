#include "RIFFReader.hpp"

#include <iostream>
#include <sstream>
#include <numeric>

#include "hexManip.hpp"
#include "binaryReader.hpp"
#include "CURReader.hpp"
#include "stringOperation.hpp"


#define endl '\n'

namespace RIFFReader {

    image::animatedImage universalImageDatas;

    void decodeRIFF(std::vector<uint8_t>& rawFileData) {
        universalImageDatas = {};
        decode(rawFileData);
    }

    void decode(std::vector<uint8_t>& data, int depth) {
        std::string depthOffset(depth, '-');
        auto it = data.begin();

        int id = toInt(it, 4, false);
        std::vector<uint8_t> body(it, data.end());
        switch(id) {
            case 0x52494646:
            case 0x52494658:
            case 0x4C495354:
                return decodeHeader(body, depth); break;
            default:
                decodeChunk(body, depth); break;
        }
    }

    void decodeHeader(std::vector<uint8_t>& data, int depth) {
        std::string depthOffset(depth, '-');
        auto it = data.begin();

        std::string idText = toAscii(it, 4);
        std::cout << depthOffset << "Header ID : " << idText << endl;

        int size = toIntRev(it, 4);
        std::cout << depthOffset << "size      : " << size << endl;
        
        std::string headerId = toAscii(it, 4);
        std::vector<uint8_t> payload = getPayload(it, size - 4);
        
        std::string extraByte = "Unused";
        if (size % 2 == 1) {
            extraByte = toHex(toInt(it, 1), 1);
        }
        std::cout << depthOffset << "nop       : " << extraByte << endl;

        bool depleted = (it == data.end());

        std::cout << endl << depthOffset << "-Chunk ID   : " << headerId << endl;

        decode(payload, depth + 1);
        if (depleted) return;
        std::vector<uint8_t> next(it, data.end());
        decode(next, depth);
    }


    void decodeChunk(std::vector<uint8_t>& data, int depth) {
        std::string depthOffset(depth, '-');
        auto it = data.begin();

        int headerType = toInt(it, 4, false);
        std::string chunkIdText = toAscii(it, 4);
        std::cout << depthOffset << "Chunk Name : " << chunkIdText << endl;
        
        int size = toIntRev(it, 4);
        std::cout << depthOffset << "size       : " << size << endl;
        
        std::vector<uint8_t> payload = getPayload(it, size);
        
        std::string extraByte = "Unused";
        if (size % 2 == 1) {
            extraByte = toHex(toInt(it, 1), 1);
        }
        std::cout << depthOffset << "nop        : " << extraByte << endl;

        bool depleted = (it == data.end());
        
        std::cout << endl;

        switch(headerType) {
            case 0x616E6968: decodeAnih(payload, depth + 1); break;
            case 0x69636F6E: decodeIcon(payload, depth + 1); break;
            case 0x73657120: decodeSeq(payload, depth + 1); break;
        }


        if (depleted) return;
        std::vector<uint8_t> next(it, data.end());
        decode(next, depth);
    }

    image::animatedImage getRIFFImages() {
        return universalImageDatas;
    }

    void decodeAnih(std::vector<uint8_t> data, int depth) {
        std::string depthOffset(depth, '-');
        auto it = data.begin();

        std::cout << depthOffset << "Data :" << endl;

        int size = toIntRev(it, 4);
        std::cout << depthOffset << "Header Size  : " << size << endl;

        int frameCount = toIntRev(it, 4);
        std::cout << depthOffset << "Frame Count  : " << frameCount << endl;
        
        int stepCount = toIntRev(it, 4);
        std::cout << depthOffset << "Step Count   : " << stepCount << endl;
        universalImageDatas.frameOrder = std::vector<uint32_t>(stepCount);
        std::iota(universalImageDatas.frameOrder.begin(), universalImageDatas.frameOrder.end(), 0);

        int width = toIntRev(it, 4);
        std::cout << depthOffset << "Image Width  : " << width << endl;

        int height = toIntRev(it, 4);
        std::cout << depthOffset << "Image Height : " << height << endl;

        int bitCount = toIntRev(it, 4);
        std::cout << depthOffset << "Bit Count    : " << bitCount << endl;

        int planeCount = toIntRev(it, 4);
        std::cout << depthOffset << "Plane Count  : " << planeCount << endl;

        int rate = toIntRev(it, 4);
        std::cout << depthOffset << "Display Rate : " << rate << endl;

        int flags = toIntRev(it, 4);
        std::cout << depthOffset << "Flags        : " << flags << endl;
        switch (flags) {
            case 0: std::cout << depthOffset << "Frames are raw data" << endl; break;
            case 1: std::cout << depthOffset << "Frames are icon or cursor data" << endl; break;
            case 2: std::cout << depthOffset << "File contains a sequence of raw data" << endl; break;
            case 3: std::cout << depthOffset << "File contains a sequence of icons or cursor data" << endl; break;
        }

        std::cout << endl;
    }

    void decodeIcon(std::vector<uint8_t> data, int depth) {
        std::string depthOffset(depth, '-');
        auto it = data.begin();

        std::cout << depthOffset << "Data :" << endl;

        int reserved = toIntRev(it, 2);
        std::cout << depthOffset << "Reserved : " << reserved << endl;

        int type = toIntRev(it, 2);
        std::cout << depthOffset << "Type : " << type << endl;

        image::image universalImageData;

        if (type == 2) {
            std::vector<uint8_t> vec(it, data.end());
            universalImageDatas.frames.push_back(CURReader::decodeCUR(vec, depth).getUniversalImageData());
        }

        std::cout << endl;
    }

    void decodeSeq(std::vector<uint8_t> data, int depth) {
        PRINTER pp;
        pp.setOffset('-', depth);
        auto it = data.begin();

        int stepCount = universalImageDatas.frameOrder.size();
        for (int i = 0; i < stepCount; i++) {
            uint32_t frameNumber = toIntRev(it, 4);
            universalImageDatas.frameOrder[i] = frameNumber;
            std::stringstream ss;
            ss << std::setw(3) << i;
            pp.print("Frame " + ss.str(), frameNumber);
        }
    }

    std::vector<uint8_t> getPayload(std::vector<uint8_t>::iterator &it, int length, bool advance)
    {
        std::vector<uint8_t> payload(it, it + length);
        if (advance) {
            std::advance(it, length);
        }
        return payload;
    }
}