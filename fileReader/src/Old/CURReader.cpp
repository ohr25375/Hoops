#include "CURReader.hpp"

#include <iostream>
#include <cmath>
#include <bitset>

#include "hexManip.hpp"
#include "stringOperation.hpp"

#define endl '\n'

#define unwrap16(n) (uint8_t)((n) & 0xff),(uint8_t)(((n) >> 8) & 0xff)
#define unwrap32(n) (uint8_t)((n) & 0xff),(uint8_t)(((n) >> 8) & 0xff),(uint8_t)(((n) >> 16) & 0xff),(uint8_t)(((n) >> 24) & 0xff)


namespace CURReader
{
    CUR decodeCUR(std::vector<uint8_t> &data, int depth) {
        CUR curData;
        auto it = data.begin();
        PRINTER pp;
        pp.setOffset('-', depth);
        pp.setEndOffset(':', 16);
        curData.imageCount = toIntRev(it, 2);
        curData.entriesInit();

        for (int i = 0; i < curData.imageCount; i++) {
            curData.entries[i].width = toIntRev(it, 1);
            curData.entries[i].height = toIntRev(it, 1);
            curData.entries[i].colorCount = toIntRev(it, 1);
            toIntRev(it, 1);
            curData.entries[i].xHotspot = toIntRev(it, 2);
            curData.entries[i].yHotspot = toIntRev(it, 2);
            curData.entries[i].sizeOfData = toIntRev(it, 4);
            curData.entries[i].headerLocation = toIntRev(it, 4);
        }
    
        toIntRev(it, 4);
        curData.width = toIntRev(it, 4);
        curData.height = toIntRev(it, 4) / 2;
        curData.planes = toIntRev(it, 2);
        curData.bitsPerPixel = toIntRev(it, 2);
        curData.compression = toIntRev(it, 4);
        curData.setImageDataByteCount(toIntRev(it, 4));
        toIntRev(it, 4);
        toIntRev(it, 4);
        toIntRev(it, 4);
        toIntRev(it, 4);
        curData.paletteInit();
        curData.colorDataInit();
        curData.maskInit();
        for (int i = 0; i < (int)curData.palette.size(); i++) {
            curData.palette[i] = CURPalette(toInt(it, 4));
            curData.palette[i].a = 0xff;
        }
        for (int i = 0; i < (int)curData.colorData.size(); i++) {
            curData.colorData[i] = toInt(it, 1);
        }
        for (int i = 0; i < (int)curData.maskData.size(); i++) {
            curData.maskData[i] = toInt(it, 1);
        }
        curData.printData(pp);
        return curData;
    }
    
    std::vector<uint8_t> CUREntry::getData() {
        std::vector<uint8_t> d(2);
        std::vector<uint8_t> data = {
            width,
            (uint8_t)(height * 2),
            colorCount,
            0,
            unwrap16(xHotspot),
            unwrap16(yHotspot),
            unwrap32(sizeOfData),
            unwrap32(headerLocation)
        };
        return data;
    }

    void CUREntry::printData(PRINTER pp) {
        PRINTER mPP = pp;
        mPP.setOffsetLength(pp.getOffset() + 1);
        mPP.print("Width", (uint32_t)width);
        mPP.print("Height", (uint32_t)height);
        mPP.print("Color Count", (uint32_t)colorCount);
        mPP.print("X Hotspot", xHotspot);
        mPP.print("Y Hotspot", yHotspot);
        mPP.print("Size of Data", sizeOfData);
        mPP.print("Header Location", headerLocation);
    }

    void CUR::entriesInit() {
        entries = std::vector<CUREntry>(imageCount);
    }

    void CUR::setImageDataByteCount(uint32_t val) {
        rowSize = floor((bitsPerPixel * width + 31) / 32) * 4;
        dataByteCount = dataByteCount == 0 ? rowSize * height * 2 : dataByteCount;
    }

    uint32_t CUR::getImageDataByteCount()
    {
        return 0;
    }

    void CUR::paletteInit() {
        int paletteLength;
        switch (bitsPerPixel) {
            case 1:
            case 2:
            case 4:
            case 8:
                isColorIndexed = true;
                paletteLength = (2 << (bitsPerPixel - 1));
                break;
            default:
                paletteLength = 0;
        }
        palette = std::vector<CURPalette>(paletteLength);
    }

    void CUR::colorDataInit() {
        int colorDataSize = rowSize * height;
        colorData = std::vector<uint8_t>(colorDataSize);
    }

    void CUR::maskInit() {
        maskRowSize = floor((width + 31) / 32) * 4;
        int maskDataSize = maskRowSize * height;
        maskData = std::vector<uint8_t>(maskDataSize);
    }

    void CUR::printData(PRINTER pp)
    {
        pp.print("Image Count", imageCount);
        for(int i = 0; i < imageCount; i++) {
            entries[i].printData(pp);
        }
        pp.print("Width", width);
        pp.print("Height", height * 2);
        pp.print("Planes", planes);
        pp.print("Bits per Pixel", bitsPerPixel);
        pp.print("Compression", compression);
        if (palette.size() != 0) {
            pp.print("Palette Data Size", palette.size() * 4);
        }
        pp.print("Row Size", rowSize);
        pp.print("Color Data Size", colorData.size());
        pp.print("Mask Row Size", maskRowSize);
        pp.print("Mask Size", maskData.size());

        std::cout << endl;

        // printMask(true);
    }

    std::vector<color::color> CUR::getPixelColors() {
        int pixelCount = width * height;
        std::vector<color::color> imagePixels(pixelCount);
        for (int y = 0; y < height; y++) {
            // row byte offset
            int dataRowIndex = rowSize * y;
            switch (bitsPerPixel) {
                case 1:
                case 2:
                case 4:
                case 8: {
                    int dataPerByte = 8 / bitsPerPixel;
                    int dataByteIndex = dataRowIndex;
                    for (int x = 0; x < width; x += dataPerByte, dataByteIndex++) {
                        int pixelIndex = x + y * width;
                        for (int i = 0; i < dataPerByte; i++) {
                            int rsRoot = 8 - bitsPerPixel;
                            int rs = rsRoot - i * bitsPerPixel;
                            int mask = (1 << bitsPerPixel) - 1;
                            imagePixels[pixelIndex + i] = palette[(colorData[dataByteIndex] >> rs) & mask];
                        }
                    }
                    break;
                }
                case 16: {
                    int dataByteIndex = dataRowIndex;
                    for (int x = 0; x < width; x++, dataByteIndex += 2) {
                        int pixelIndex = x + y * width;
                        uint16_t mixed = colorData[dataByteIndex] << 8 + colorData[dataByteIndex + 1];
                        // xrrrrrgg gggbbbbb
                        int b = ((mixed >> 0) & 0b11111) * 8;
                        int g = ((mixed >> 5) & 0b11111) * 8;
                        int r = ((mixed >> 10) & 0b11111) * 8;
                        imagePixels[pixelIndex] = color::color(r, g, b);
                    }
                    break;
                }
                case 32: {
                    int dataByteIndex = dataRowIndex;
                    for (int x = 0; x < width; x++, dataByteIndex += 4) {
                        int pixelIndex = x + y * width;
                        // bbbbbbbb gggggggg rrrrrrrr aaaaaaaa
                        int b = colorData[dataByteIndex + 0];
                        int g = colorData[dataByteIndex + 1];
                        int r = colorData[dataByteIndex + 2];
                        int a = colorData[dataByteIndex + 3];
                        color::color c(r,g,b,a);
                        imagePixels[pixelIndex] = c;
                    }
                    break;
                }
            }
        }

        // apply mask
        for (int y = 0; y < height; y++) {
            int xMax = width / 8;
            int yOffset = y * maskRowSize;
            int yImageOffset = y * width;
            for (int x = 0; x < xMax; x++) {
                int maskIndex = x + yOffset;
                std::bitset<8> maskBits(maskData[maskIndex]);
                for (int i = 0; i < 8; i++) {
                    int imageIndex = i + x * 8 + yImageOffset;
                    if (!maskBits[8 - i - 1]) continue;
                    imagePixels[imageIndex].a = 0;
                }
            }
        }

        // flip y
        for (int y = 0; y < height / 2; y++) {
            int swapHeightIndex = height - 1 - y;
            for (int x = 0; x < width; x++) {
                int ai = x + y * width;
                int bi = x + swapHeightIndex * width;
                auto& a = imagePixels[ai];
                auto& b = imagePixels[bi];

                std::swap(a, b);
            }
        }
        return imagePixels;
    }

    image::image CUR::getUniversalImageData() {
        image::image data;
        data.width = width;
        data.height = height;
        data.pixelData = getPixelColors();
        return data;
    }

    void CUR::printMask(bool flipY) {
        for (int y = 0; y < height; y++) {
            int xMax = width / 8;
            int yOffset = y * maskRowSize;
            if (flipY) {
                yOffset = (height - 1 - y) * maskRowSize;
            }
            for (int x = 0; x < xMax; x++) {
                int index = x + yOffset;
                std::cout << std::bitset<8>(maskData[index]) << (x == xMax - 1 ? endl : ' ');
            }
        }
    }

    CURPalette::CURPalette(uint32_t brg0)
    {
        b = (brg0 >> 24) & 0xff;
        g = (brg0 >> 16) & 0xff;
        r = (brg0 >> 8) & 0xff;
        a = (brg0 >> 0) & 0xff;
    }

    CURPalette::operator color::color() const {
        return color::color(r, g, b, a);
    }
}
