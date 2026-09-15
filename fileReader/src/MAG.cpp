#include "MAG.hpp"

#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>

#include "hexManipulator.hpp"

namespace MAG {
    CHECKSUM fillChecksum(const uint8_t* data) {
        CHECKSUM checksum;
        checksum.mode    = std::vector<FTYPE::BYTE>(data + 0, data + 8);
        std::cout << "filled mode\n";
        checksum.machine = std::vector<FTYPE::BYTE>(data + 8, data + 12);
        std::cout << "filled machine\n";
        checksum.user    = std::vector<FTYPE::BYTE>(data + 13, data + 31);
        std::cout << "filled user\n";
        auto memoLength  = 32;
        while (*(data + memoLength)) memoLength++;
        checksum.memo = std::vector<FTYPE::BYTE>(data + 32, data + memoLength);
        std::cout << "filled memo\n";
        return checksum;
    }
    HEADER fillHeader(const uint8_t* data) {
        HEADER header;
        header.top         = *((FTYPE::BYTE*)(data + 0));
        header.machine     = *((FTYPE::BYTE*)(data + 1));
        header.flags       = *((FTYPE::BYTE*)(data + 2));
        header.mode        = *((FTYPE::BYTE*)(data + 3));
        header.xStart      = *((FTYPE::WORD*)(data + 4));
        header.yStart      = *((FTYPE::WORD*)(data + 6));
        header.xEnd        = *((FTYPE::WORD*)(data + 8));
        header.yEnd        = *((FTYPE::WORD*)(data + 10));
        header.flagAOffset = *((FTYPE::DWORD*)(data + 12));
        header.flagBOffset = *((FTYPE::DWORD*)(data + 16));
        header.flagBSize   = *((FTYPE::DWORD*)(data + 20));
        header.pixelOffset = *((FTYPE::DWORD*)(data + 24));
        header.pixelSize   = *((FTYPE::DWORD*)(data + 28));
        return header;
    }
    PALETTE fillPalette(const uint8_t* data, const int colorCount) {
        std::cout << "MAG: filling palette with " << (int)colorCount << " colors...";
        PALETTE palette;
        palette.colors = std::vector<COLOR::COLOR>(colorCount);
        for (int i = 0; i < colorCount; i++) {
            palette.colors[i].g = *(data + i * 3);
            palette.colors[i].r = *(data + i * 3 + 1);
            palette.colors[i].b = *(data + i * 3 + 2);
        }
        std::cout << " done\n";
        return palette;
    }
    FLAGAB fillFlagAB(const uint8_t* data, const size_t flagASize, const size_t flagBSize) {
        FLAGAB flagAB;
        flagAB.flagA = std::vector<uint8_t>(data, data + flagASize);
        flagAB.flagB = std::vector<uint8_t>(data + flagASize, data + flagASize + flagBSize);
        return flagAB;
    }
    PIXEL_DATA fillPixelData(const uint8_t* data, const size_t size) {
        PIXEL_DATA pixelData;
        pixelData.data = std::vector<uint8_t>(data, data + size);
        return pixelData;
    }
    void LOADER::printMAG(const MAG& mag) const {
        std::cout << "Checksum\n";
        std::cout << "mode: " << mag.checksum.mode.data() << mag.checksum.mode.size() << '\n';
        std::cout << "machine: " << mag.checksum.machine.data() << mag.checksum.machine.size() << '\n';
        std::cout << "user: " << mag.checksum.user.data() << mag.checksum.user.size() << '\n';
        std::cout << "memo: " << mag.checksum.memo.data();
        std::cout << "(" << mag.checksum.memo.size() << ")";
        std::cout << "\n";
        auto& header = mag.header;
        std::cout << "Header\n";
        std::cout << "top: " << (uint32_t)header.top << '\n';
        std::cout << "machine: " << (uint32_t)header.machine << '\n';
        std::cout << "flags: " << (uint32_t)header.flags << '\n';
        std::cout << "mode: " << (uint32_t)header.mode << '\n';
        std::cout << "xStart: " << (uint32_t)header.xStart << '\n';
        std::cout << "yStart: " << (uint32_t)header.yStart << '\n';
        std::cout << "xEnd: " << (uint32_t)header.xEnd << '\n';
        std::cout << "yEnd: " << (uint32_t)header.yEnd << '\n';
        std::cout << "flagAOffset: " << (uint32_t)header.flagAOffset << '\n';
        std::cout << "flagBOffset: " << (uint32_t)header.flagBOffset << '\n';
        std::cout << "flagASize: " << header.flagBOffset - header.flagAOffset << '\n';
        std::cout << "flagBSize: " << (uint32_t)header.flagBSize << '\n';
        std::cout << "pixelOffset: " << (uint32_t)header.pixelOffset << '\n';
        std::cout << "pixelSize: " << (uint32_t)header.pixelSize << '\n';
        std::cout << "\n";
        auto colorCount = (header.mode & 0x80 ? 256 : 16);
        std::cout << "colors: " << colorCount << '\n';
        auto pixelUnitLog = (header.mode & 0x80 ? 1 : 2);
        std::cout << "pixelUnitLog: " << pixelUnitLog << '\n';
        std::cout << "\n";
        std::cout << "Palette\n";
        for (size_t i = 0; i < colorCount; i++) {
            auto g = hexManipulator::toHexString(mag.palette.colors[i].g);
            auto r = hexManipulator::toHexString(mag.palette.colors[i].r);
            auto b = hexManipulator::toHexString(mag.palette.colors[i].b);
            std::cout << "color[0x" << hexManipulator::toHexString((char)i) << "]: G 0x" << g;
            std::cout << " :R 0x" << r;
            std::cout << " :B 0x" << b;
            std::cout << " :RGB 0x" << r << g << b << '\n';
        }
        std::cout << "\n";
        auto width = (header.xEnd & 0xfff8 | 7) - (header.xStart & 0xfff8) + 1;
        std::cout << "width: " << width << '\n';
        auto height = header.yEnd - header.yStart + 1;
        std::cout << "height: " << height << '\n';
        auto flagSize = width >> (pixelUnitLog + 1);
        std::cout << "flagSize: " << flagSize << '\n';
    }
    MAG LOADER::loadMAG(const uint8_t* data) const {
        std::cout << "\n=============================================================\n";
        std::cout << "MAG LOADER: loading mag...\n";
        MAG magData;
        magData.checksum  = fillChecksum(data);
        auto headerOffset = data;
        for (; *headerOffset; headerOffset++);
        magData.header    = fillHeader(headerOffset);
        magData.palette   = fillPalette(headerOffset + 32, (magData.header.mode & 0x80 ? 256 : 16));
        auto flagASize    = magData.header.flagBOffset - magData.header.flagAOffset;
        magData.flagAB    = ::MAG::fillFlagAB(headerOffset + magData.header.flagAOffset, flagASize, magData.header.flagBSize);
        magData.pixelData = fillPixelData(headerOffset + magData.header.pixelOffset, magData.header.pixelSize);
        std::cout << "MAG LOADER: loaded mag\n";
        std::cout << "=============================================================\n\n";
        return magData;
    }
    std::vector<COLOR::COLOR> LOADER::extractPixelData(const MAG& mag) const {
        auto& palette = mag.palette.colors;
        std::vector<COLOR::COLOR> pixelData;

        auto indexedData = extractIndexedData(mag);
        pixelData        = std::vector<COLOR::COLOR>(indexedData.size());
        for (auto i = 0; i < pixelData.size(); i++) {
            pixelData[i] = palette[indexedData[i]];
        }

        return pixelData;
    }
    std::vector<uint8_t> LOADER::_correct200LineIndexedData(const std::vector<uint8_t>& data, const int pitch) const {
        std::cout << "MAG LOADER: 200 mode detected...";
        auto height = (int)data.size() / pitch;
        auto result = std::vector<uint8_t>(data.size() * 2, 0);
        std::cout << " correcting to " << pitch << " x " << height * 2 << "...";

        for (auto y = height * 2 - 1; y >= 0; y--) {
            for (auto x = 0; x < pitch; x++) {
                auto destinationIndex    = x + y * pitch;
                auto dataIndex           = x + (y / 2) * pitch;
                result[destinationIndex] = data[dataIndex];
            }
        }
        std::cout << "done\n";
        return result;
    }
    std::vector<uint8_t> LOADER::extractIndexedData(const MAG& mag) const {
        std::cout << "\n=============================================================\n";
        std::cout << "MAG LOADER: extracting indexed data from mag...\n";
        auto data = _extractIndexedData(mag);
        if (mag.header.mode & 1) {
            const auto width = (mag.header.xEnd & 0xfff8 | 7) - (mag.header.xStart & 0xfff8) + 1;
            data = _correct200LineIndexedData(data, width);
        }
        std::cout << "MAG LOADER: done\n";
        std::cout << "=============================================================\n\n";
        return data;
    }
    std::vector<uint8_t> LOADER::_extractIndexedData(const MAG& mag) const {
        std::vector<uint8_t> indexedData;
        auto& header = mag.header;
        struct bitVector {
            std::vector<uint8_t> vector;
            size_t index = 0;
            bitVector(const size_t size = 0, const uint8_t initial_value = 0) {
                vector = std::vector<uint8_t>(size, initial_value);
            }
            bitVector(const std::vector<uint8_t>& vector) {
                this->vector = vector;
            }
            bool operator[](const size_t i) const {
                return vector[i / 8] & (0b10000000 >> (i % 8));
            }
        };
        const bitVector flagA = mag.flagAB.flagA;
        auto& flagB           = mag.flagAB.flagB;
        auto& magPixelData    = mag.pixelData.data;

        const auto width  = (header.xEnd & 0xfff8 | 7) - (header.xStart & 0xfff8) + 1;
        const auto height = header.yEnd - header.yStart + 1;
        const int copyX[] = {0, 1, 2, 4, 0, 1, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0};
        const int copyY[] = {0, 0, 0, 0, 1, 1, 2, 2, 2, 4, 4, 4, 8, 8, 8, 16};
        auto copyIndex    = std::vector<int>(16);
        auto pixelUnitLog = (header.mode & 0x80 ? 1 : 2);
        for (auto i = 0; i < 16; i++) {
            copyIndex[i] = -(copyY[i] * width + (copyX[i] << pixelUnitLog));
        }

        const auto copySize = 1 << pixelUnitLog;
        const auto flagSize = width >> (pixelUnitLog + 1);
        const auto colors   = (header.mode & 0x80 ? 256 : 16);
        auto mask           = 0x80;
        auto flagAPos       = 0;
        auto flagBPos       = 0;
        auto src            = 0;
        auto dest           = 0;
        auto flagBuf        = std::vector<uint8_t>(flagSize);
        indexedData         = std::vector<uint8_t>(width * height);

        std::cout << "MAG LOADER: Creating pixel data for image size [" << width << "x" << height << "]";

        std::cout << " Starting extraction...";

        for (auto y = 0; y < height; y++) {
            for (auto x = 0; x < flagSize; x++) {
                if (!flagA[flagAPos++]) continue;
                flagBuf[x] ^= flagB[flagBPos++];
            }
            for (auto x = 0; x < flagSize; x++) {
                auto flag = flagBuf[x];
                for (auto i = 1; i >= 0; i--) {
                    auto v = (flag >> (4 * i)) & 0xf;
                    if (!v) {
                        if (colors == 16) {
                            indexedData[dest++] = magPixelData[src] >> 4;
                            indexedData[dest++] = magPixelData[src++] & 0xf;
                            indexedData[dest++] = magPixelData[src] >> 4;
                            indexedData[dest++] = magPixelData[src++] & 0xf;
                        } else {
                            indexedData[dest++] = magPixelData[src++];
                            indexedData[dest++] = magPixelData[src++];
                        }
                    } else {
                        auto copySrc = dest + copyIndex[v];
                        for (auto i = 0; i < copySize; i++) {
                            indexedData[dest + i] = indexedData[copySrc + i];
                        }
                        dest += copySize;
                    }
                }
            }
        }
        std::cout << " read successful.\n";
        return indexedData;
    }

    template <class T, class T2>
    std::basic_stringbuf<T2>& put(std::basic_stringbuf<T2>& buf, const T& var) {
        buf.sputn(reinterpret_cast<const T2*>(&var), sizeof(var));
        return buf;
    }

    std::vector<uint8_t> LOADER::createData16(const std::vector<uint8_t>& indexedData, const uint16_t pitch, const std::vector<COLOR::COLOR>& palette, const CHECKSUM& checksum) const {
        std::cout << "\n=============================================================\n";
        std::cout << "MAG LOADER: Initializing 640x400 16 color data creation...\n";
        if (palette.size() != 16) {
            std::cout << "\nMAG LOADER: Data mismatch: palette size is not 16 (found palette with size " << palette.size() << ")\n";
            return std::vector<uint8_t>(0);
        }
        const auto& width = pitch;
        if (width != 640) {
            std::cout << "\nMAG LOADER: Size mismatch: width is not 640 (found width = " << width << ")\n";
            return std::vector<uint8_t>(0);
        }
        const auto height = indexedData.size() / pitch;
        if (height != 400) {
            std::cout << "\nMAG LOADER: Size mismatch: height is not 400 (found height = " << height << ")\n";
            return std::vector<uint8_t>(0);
        }
        const auto pixelDataWidth      = width / 2;
        std::vector<uint8_t> pixelData = compressNibbles(indexedData);

        std::vector<uint8_t> flagBuffer(0);
        std::vector<uint8_t> pixelBuffer(0);
        fillFlagAndPixelBuffer(pixelData, flagBuffer, pixelBuffer, pitch);
        formatFlagBuffer(flagBuffer, height);

        std::vector<uint8_t> flagA;
        std::vector<uint8_t> flagB;
        fillFlagAB(flagBuffer, flagA, flagB);

        std::vector<uint8_t> paletteData = expandPalette(palette);
        std::vector<uint8_t> data        = createDataStream(flagA, flagB, pixelBuffer, paletteData, checksum);

        std::cout << "MAG LOADER: Successfully created 640x400 16 color data stream\n";
        std::cout << "=============================================================\n\n";
        return data;
    }
    std::vector<uint8_t> LOADER::createDataStream(const std::vector<uint8_t>& flagA, const std::vector<uint8_t>& flagB, const std::vector<uint8_t>& pixelBuffer, const std::vector<uint8_t>& paletteData, const CHECKSUM& checksum) const {
        std::basic_stringbuf<uint8_t> dataStream;

        dataStream.sputn(checksum.mode.data(), checksum.mode.size());
        dataStream.sputn(checksum.machine.data(), checksum.machine.size());
        put(dataStream, ' ');
        dataStream.sputn(checksum.user.data(), checksum.user.size());
        put(dataStream, ' ');
        dataStream.sputn(checksum.memo.data(), checksum.memo.size());

        const auto headerOffset = 32;

        FTYPE::BYTE header_top          = 0;
        FTYPE::BYTE header_machine      = 255;
        FTYPE::BYTE header_flags        = 0;
        FTYPE::BYTE header_mode         = 0;
        FTYPE::WORD header_xStart       = 0;
        FTYPE::WORD header_yStart       = 0;
        FTYPE::WORD header_xEnd         = 639;
        FTYPE::WORD header_yEnd         = 399;
        FTYPE::DWORD header_flagAOffset = 32 + paletteData.size();
        FTYPE::DWORD header_flagBOffset = header_flagAOffset + flagA.size();
        FTYPE::DWORD header_flagBSize   = flagB.size();
        FTYPE::DWORD header_pixelOffset = header_flagBSize + header_flagBOffset;
        FTYPE::DWORD header_pixelSize   = pixelBuffer.size();
        put(dataStream, header_top);
        put(dataStream, header_machine);
        put(dataStream, header_flags);
        put(dataStream, header_mode);
        put(dataStream, header_xStart);
        put(dataStream, header_yStart);
        put(dataStream, header_xEnd);
        put(dataStream, header_yEnd);
        put(dataStream, header_flagAOffset);
        put(dataStream, header_flagBOffset);
        put(dataStream, header_flagBSize);
        put(dataStream, header_pixelOffset);
        put(dataStream, header_pixelSize);

        std::basic_string bs      = dataStream.str();
        std::vector<uint8_t> data = std::vector<uint8_t>(bs.begin(), bs.end());
        data.insert(data.end(), paletteData.begin(), paletteData.end());
        data.insert(data.end(), flagA.begin(), flagA.end());
        data.insert(data.end(), flagB.begin(), flagB.end());
        data.insert(data.end(), pixelBuffer.begin(), pixelBuffer.end());

        return data;
    }

    std::vector<uint8_t> LOADER::expandPalette(const std::vector<COLOR::COLOR>& palette) const {
        const auto paletteSize = palette.size();
        std::vector<uint8_t> paletteData(paletteSize * 3);
        for (auto i = 0; i < paletteSize; i++) {
            paletteData[i * 3]     = palette[i].g;
            paletteData[i * 3 + 1] = palette[i].r;
            paletteData[i * 3 + 2] = palette[i].b;
        }
        return paletteData;
    }
    std::vector<uint8_t> LOADER::compressNibbles(const std::vector<uint8_t>& nibbles) const {
        const auto byteCount = nibbles.size() / 2;
        std::vector<uint8_t> bytes(byteCount);
        for (auto i = 0; i < byteCount; i++) {
            bytes[i] = (nibbles[i * 2] << 4) | nibbles[i * 2 + 1];
        }
        return bytes;
    }
    int LOADER::fillFlagAndPixelBuffer(const std::vector<uint8_t>& pixelData, std::vector<uint8_t>& flagBuffer, std::vector<uint8_t>& pixelBuffer, const uint16_t pitch) const {
        std::cout << "MAG LOADER: creating copyOffset... ";
        const auto pixelDataWidth    = pitch / 2;
        const int copyCheckOrders[]  = {1, 4, 5, 6, 7, 9, 10, 2, 8, 11, 12, 13, 14, 3, 15};
        const int copyCheckOrderSize = 15;
        const int copyX[]            = {0, 1, 2, 4, 0, 1, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0};
        const int copyY[]            = {0, 0, 0, 0, 1, 1, 2, 2, 2, 4, 4, 4, 8, 8, 8, 16};
        auto copyOffsets             = std::vector<int>(16);
        for (auto i = 0; i < 16; i++) {
            copyOffsets[i] = (copyY[i] * pixelDataWidth + (copyX[i] << 1));
        }
        std::cout << "done\n";

        std::cout << "MAG LOADER: creating raw flag buffer... ";
        std::vector<uint8_t> rawFlagBuffer(0);
        const auto pixelBufferPitch = pitch >> 2;
        const auto pixelDataSize    = pixelData.size();
        for (auto i = 0; i < pixelDataSize / 2; i++) {
            auto flag          = 0;
            const auto x       = i % pixelBufferPitch;
            const auto y       = i / pixelBufferPitch;
            const auto pixelA1 = pixelData[i * 2];
            const auto pixelA2 = pixelData[i * 2 + 1];
            for (auto c = 0; c < copyCheckOrderSize; c++) {
                const auto copyIndex = copyCheckOrders[c];
                if (x < copyX[copyIndex]) continue;
                if (y < copyY[copyIndex]) continue;
                const auto copyOffset = copyOffsets[copyIndex];
                const auto pixelB1    = pixelData[i * 2 - copyOffset];
                const auto pixelB2    = pixelData[i * 2 + 1 - copyOffset];
                if (pixelA1 != pixelB1 || pixelA2 != pixelB2) continue;

                flag = copyIndex;
                break;
            }
            rawFlagBuffer.push_back(flag);
            if (flag == 0) {
                pixelBuffer.push_back(pixelA1);
                pixelBuffer.push_back(pixelA2);
            }
        }
        flagBuffer = compressNibbles(rawFlagBuffer);

        std::cout << "Flag Buffer: " << flagBuffer.size() << " done\n";
        return 0;
    }

    int LOADER::formatFlagBuffer(std::vector<uint8_t>& flagBuffer, const uint16_t height) const {
        const auto flagBufferSize  = flagBuffer.size();
        const auto flagBufferPitch = flagBufferSize / height;
        for (auto i = flagBufferSize - 1; i >= 0; i--) {
            if (i < flagBufferPitch) break;
            flagBuffer[i] ^= flagBuffer[i - flagBufferPitch];
        }
        return 0;
    }

    int LOADER::fillFlagAB(const std::vector<uint8_t>& flagBuffer, std::vector<uint8_t>& flagA, std::vector<uint8_t>& flagB) const {
        std::cout << "MAG LOADER: filling FlagAB... ";
        const auto flagBufferSize = flagBuffer.size();
        flagA                     = std::vector<uint8_t>(flagBufferSize / 8, 0);
        flagB                     = std::vector<uint8_t>(0);

        for (auto i = 0; i < flagBufferSize; i++) {
            const auto flagAIndex  = i / 8;
            const auto flagAOffset = i % 8;
            if (flagBuffer[i]) {
                flagA[flagAIndex] |= 0b10000000 >> flagAOffset;
                flagB.push_back(flagBuffer[i]);
            }
        }
        if (flagB.size() & 1) {
            flagB.push_back(0);
        }

        std::cout << "FlagA: " << flagA.size() << " | FlagB: " << flagB.size() << " done\n";
        return 0;
    }
}  // namespace MAG