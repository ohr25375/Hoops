#pragma once
#ifndef RIFF_FILE
#define RIFF_FILE

#include <cstdint>
#include <array>
#include <vector>
#include <filesystem>

namespace RIFF {
    typedef uint32_t DWORD;
    typedef uint16_t WORD;
    typedef uint8_t BYTE;
    
    typedef DWORD FOURCC;
    
    typedef FOURCC CKID;
    typedef DWORD CKSIZE;

    const CKID RIFF_ID = 0x46464952;
    const CKID RIFF_LIST_ID = 0x5453494c;
    
    class CK {
    public:
        CKID ckID;

        CK(CKID ckID, CKSIZE ckSize);
        virtual ~CK() = default;

        CKSIZE getSize();
        std::vector<BYTE> packData();

    protected:
        CKSIZE ckSize;
        std::vector<BYTE> ckData;
        void insertData(std::vector<BYTE>::iterator& it);
    };

    std::vector<BYTE> packToRIFF(const std::vector<BYTE>& data);
    int32_t extractDataAsRIFF(std::vector<BYTE>::iterator& riffData, std::vector<BYTE>& data);
    int32_t getDataPosition(std::vector<BYTE>::iterator& riffData, std::vector<BYTE>::iterator& data);
    int32_t writeToFile(std::vector<BYTE>& data, std::filesystem::path path);

    void printCK(CK& ck);
}

#endif