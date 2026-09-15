#include "ANI.hpp"

#include <iostream>
#include <numeric>

#include "hexManipulator.hpp"
#include "binaryReader.hpp"

namespace ANI {

    bool isDebugMode = false;

    ANI_HEADER::ANI_HEADER() : RIFF::CK(ANI_HEADER_ID, ANI_HEADER_SIZE) {
        hdSize = ANI_HEADER_SIZE;
    }
    ANI_HEADER::ANI_HEADER(RIFF::CKSIZE ckSize, std::vector<RIFF::BYTE>::iterator& data) : RIFF::CK(ANI_HEADER_ID, ckSize) {
        this->ckSize = ckSize;
        insertData(data);
        hdSize      = hexManipulator::getValue<RIFF::CKSIZE>(data);
        frameCount  = hexManipulator::getValue<RIFF::DWORD >(data);
        stepCount   = hexManipulator::getValue<RIFF::DWORD >(data);
        if (stepCount == 0) stepCount = frameCount;
        imageWidth  = hexManipulator::getValue<RIFF::DWORD >(data);
        imageHeight = hexManipulator::getValue<RIFF::DWORD >(data);
        bitCount    = hexManipulator::getValue<RIFF::DWORD >(data);
        planeCount  = hexManipulator::getValue<RIFF::DWORD >(data);
        rate        = hexManipulator::getValue<RIFF::DWORD >(data);
        flags       = hexManipulator::getValue<RIFF::DWORD >(data);
    }
    void ANI_HEADER::print() {
        RIFF::printCK(*this);
        std::cout << "Header Size  : " << hdSize << '\n';
        std::cout << "Frame Count  : " << frameCount << '\n';
        std::cout << "Step Count   : " << stepCount << '\n';
        std::cout << "Image Width  : " << imageWidth << '\n';
        std::cout << "Image Height : " << imageHeight << '\n';
        std::cout << "Bit Count    : " << bitCount << '\n';
        std::cout << "Plane Count  : " << planeCount << '\n';
        std::cout << "Rate         : " << rate << '\n';
        std::cout << "Flags        : " << flags << '\n';
    }

    ANI_ICON::ANI_ICON() : RIFF::CK(ANI_ICON_ID, ANI_ICON_MINIMAL_SIZE) {}
    ANI_ICON::ANI_ICON(RIFF::CKSIZE ckSize, std::vector<RIFF::BYTE>::iterator &data) : RIFF::CK(ANI_ICON_ID, ckSize) {
        this->ckSize = ckSize;
        insertData(data);
        cursorData = CUR::CUR(data);
    }
    void ANI_ICON::print() {
        std::cout << "Chunk ID     : 0x" << hexManipulator::toHexString(ANI_ICON_ID) << '\n';
        cursorData.print();
    }

    ANI_FRAM::ANI_FRAM() : RIFF::CK(ANI_FRAM_ID, ANI_FRAM_MINIMAL_SIZE) {}
    void ANI_FRAM::insertFRAM(RIFF::CKSIZE ckSize, std::vector<RIFF::BYTE>::iterator &data) {
        this->ckSize = ckSize;
        insertData(data);
        uint32_t frameCount = (uint32_t)icons.size();
        for (uint32_t i = 0; i < frameCount; i++) {
            if (hexManipulator::getValue<RIFF::CKID>(data) != ANI_ICON_ID) {
                throw std::runtime_error("invalid file format : missing icon header\n");
            }
            uint32_t headerSize = hexManipulator::getValue<RIFF::CKSIZE>(data);
            auto it = data;
            icons[i] = ANI_ICON(headerSize, data);
       }
    }
    void ANI_FRAM::print() {
        std::cout << "Header ID    : 0x" << hexManipulator::toHexString(RIFF::RIFF_LIST_ID) << '\n';
        RIFF::printCK(*this);
        uint32_t frameCount = (uint32_t)icons.size();
        for (uint32_t i = 0; i < frameCount; i++) {
            std::cout << "Frame " << i << " : " << '\n';
            icons[i].print();
        }
    }

    ANI_SEQ::ANI_SEQ() : RIFF::CK(ANI_SEQ_ID, 0){
        frameOrder = std::vector<RIFF::DWORD>(0);
    }
    void ANI_SEQ::insertSequence(RIFF::CKSIZE ckSize, std::vector<RIFF::BYTE>::iterator& data) {
        this->ckSize = ckSize;
        insertData(data);
        uint32_t length = ckSize / 4;
        for (uint32_t i = 0; i < length; i++) {
            frameOrder[i] = hexManipulator::getValue<RIFF::DWORD>(data);
        }
    }
    void ANI_SEQ::print() {
        RIFF::printCK(*this);
        std::cout << "Frame order  : ";
        uint32_t length = (uint32_t)frameOrder.size();
        for (uint32_t i = 0; i < length; i++) {
            std::cout << frameOrder[i] << (i == length - 1 ? '\n' : ',');
        }
    }

    ANI_RATE::ANI_RATE() : RIFF::CK(ANI_RATE_ID, 0) {
        timing = std::vector<RIFF::DWORD>(0, 15);
    }
    void ANI_RATE::insertRate(RIFF::CKSIZE ckSize, std::vector<RIFF::BYTE>::iterator &data) {
        this->ckSize = ckSize;
        insertData(data);
        uint32_t length = ckSize / 4;
        for (uint32_t i = 0; i < length; i++) {
            timing[i] = hexManipulator::getValue<RIFF::DWORD>(data);
        }
    }
    void ANI_RATE::print() {
        RIFF::printCK(*this);
        std::cout << "Timing       : ";
        uint32_t length = (uint32_t)timing.size();
        for (uint32_t i = 0; i < length; i++) {
            std::cout << timing[i] << (i == length - 1 ? '\n' : ',');
        }
    }

    ANI::ANI(std::vector<RIFF::BYTE> &data, bool debug) {
        isDebugMode = debug;
        try {
            std::vector<RIFF::BYTE>::iterator riffData;
            auto it = data.begin();
            if (RIFF::getDataPosition(it, it)) {
                throw std::runtime_error("invalid file format : missing RIFF header\n");
            }

            if (isDebugMode) std::cout << "Confirmed RIFF\n";


            if (hexManipulator::getValue<RIFF::CKID>(it) != ANI_ID) {
                throw std::runtime_error("invalid file format : missing ACON header\n");
            }

            if (isDebugMode) std::cout << "Confirmed ACON\n";

            readACON(it, data);
        } catch (std::exception& e) {
            std::cout << e.what() << '\n';
        }
    }
    void ANI::readACON(std::vector<RIFF::BYTE>::iterator &it, std::vector<RIFF::BYTE> &data) {
        unread = std::vector<RIFF::BYTE>(0);

        bool error = false;
        while (std::distance(it, data.end()) != 0) {
            uint32_t distance = std::distance(it, data.end());
            uint32_t ckID = hexManipulator::getValue<RIFF::CKID>(it);
            uint32_t ckSize = hexManipulator::getValue<RIFF::CKSIZE>(it);
            auto p = it;
            if (isDebugMode) std::cout << "Found Chunk ID : 0x" << hexManipulator::toHexString(ckID) << " with size " << ckSize << ", " << distance << " left" << '\n';
            switch (ckID) {
                case ANI_HEADER_ID: {
                    header = ANI_HEADER(ckSize, it);
                    frames.icons = std::vector<ANI_ICON>(header.frameCount);
                    sequence.frameOrder = std::vector<RIFF::DWORD>(header.stepCount, 0);
                    if (header.frameCount == header.stepCount) {
                        std::iota(std::begin(sequence.frameOrder), std::end(sequence.frameOrder), 0);
                    }
                    rate.timing = std::vector<RIFF::DWORD>(header.stepCount, header.rate);
                    break;
                }
                case RIFF::RIFF_LIST_ID: {
                    uint32_t headerID = hexManipulator::getValue<RIFF::CKID>(it);
                    switch (headerID) {
                        case ANI_FRAM_ID: {
                            frames.insertFRAM(ckSize, it);
                            break;
                        }
                        default: {
                            it -= 16;
                            unread.insert(unread.end(), it, it + 8 + ckSize);
                            it += 8 + ckSize;
                        }
                    }
                    break;
                }
                case ANI_SEQ_ID: {
                    sequence.insertSequence(ckSize, it);
                    break;
                }
                case ANI_RATE_ID: {
                    rate.insertRate(ckSize, it);
                    break;
                }
                default: {
                    it -= 8;
                    unread.insert(unread.end(), it, it + 8 + ckSize);
                    it += 8 + ckSize;
                    error = true;
                }
            }
            it = p + ckSize;
        }
    }
    void ANI::print() {
        std::cout << "Header ID    : 0x" << hexManipulator::toHexString(ANI_ID) << '\n';
        header.print();
        sequence.print();
        rate.print();
        frames.print();
        
        binaryReader::printBytes(unread);
    }
}