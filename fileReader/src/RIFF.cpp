#include "RIFF.hpp"

#include "hexManipulator.hpp"
#include <iostream>
#include <fstream>

namespace RIFF {
    CK::CK(CKID ckID, CKSIZE ckSize) : ckID(ckID), ckSize(ckSize) {
        ckData = std::vector<BYTE>(ckSize);
    }
    std::vector<BYTE> CK::packData() {
        std::vector<BYTE> res;
        hexManipulator::packBytes(ckID, res);
        auto size = getSize();
        hexManipulator::packBytes(size, res);
        res.insert(std::end(res), std::begin(ckData), std::end(ckData));
        if (size % 2 == 1) {
            res.push_back(0);
        }
        return res;
    }
    void CK::insertData(std::vector<BYTE>::iterator &it) {
        ckData = std::vector<BYTE>(it, it + ckSize);
    }
    CKSIZE CK::getSize()
    {
        ckSize = ckData.size();
        return ckSize;
    }
    std::vector<BYTE> packToRIFF(const std::vector<BYTE> &data) {
        std::vector<BYTE> res;
        hexManipulator::packBytes(RIFF_ID, res);
        RIFF::DWORD size = data.size();
        hexManipulator::packBytes(size, res);
        res.insert(std::end(res), std::begin(data), std::end(data));
        if (size % 2 == 1) {
            res.push_back(0);
        }
        return res;
    }
    int32_t extractDataAsRIFF(std::vector<BYTE>::iterator& riffData, std::vector<BYTE>& data) {
        if (hexManipulator::getValue<CKID>(riffData) != RIFF_ID) {
            std::cout << "RIFF : File is not RIFF" << '\n';
            return -1;
        }
        CKSIZE size = hexManipulator::getValue<CKSIZE>((riffData));
        data = std::vector<BYTE>(riffData, riffData + size);
        if (data.size() != size) {
            std::cout << "RIFF : Size mismatch. Expected (" << size << "), found (" << data.size() << ")" << '\n';
            return -1;
        }
        return 0;
    }
    int32_t getDataPosition(std::vector<BYTE>::iterator &riffData, std::vector<BYTE>::iterator &data) {
        uint32_t ckID = hexManipulator::getValue<CKID>(riffData);
        if (ckID != RIFF_ID) {
            return -1;
        }
        CKSIZE size = hexManipulator::getValue<CKSIZE>((riffData));
        data = riffData;
        return 0;
    }
    int32_t writeToFile(std::vector<BYTE> &data, std::filesystem::path path) {
        std::ofstream fs(path, std::ios::out | std::ios::binary);
        fs.write(reinterpret_cast<char*>(data.data()), sizeof(uint8_t) * data.size());
        fs.close();
        return 0;
    }
    void printCK(CK &ck)
    {
        std::cout << "Chunk ID     : 0x" << hexManipulator::toHexString(ck.ckID) << '\n';
        std::cout << "Chunk Size   : " << ck.getSize() << '\n';
    }
}