#pragma once

#include "saveFile_base.hpp"
#include <vector>

namespace SAVE_FILE_V0001 {
// VALUE        SIZE        (72)
// SAVE         4 char
// _fileSize    4 uint32    (64)
// RING         4 char
// _version     2 uint16    VER.1
// -pad         2 null
//--CHUNKS--                (56)
//--CONFIG--                (16)
// CONF         4 char
// _confSize    4 uint32    (8)
// _volume      1 uint8(p)  (xbbbxfff)
// _track       1 uint8
// _colorA      2 packed    (xxxxrrrr ggggbbbb)
// _colorB      2 packed    (xxxxrrrr ggggbbbb)
// _preset      1 uint8
// _screenSize  1 uint8
//--SCORES--                (40)
// SCOR         4 char
// _scorSize    4 uint32    (32)
// _score1      4 uint32
// _score1Name  6 char
// _score2      4 uint32
// _score2Name  6 char
// _score3      4 uint32
// _score2Name  6 char
// -pad         2 null

struct CONFIG : public SAVE_FILE_BASE::CHUNK {
    uint8_t volume = 2 << 4 | 2;
    uint8_t track = 1;
    uint16_t colorA = 0x0fff;
    uint16_t colorB = 0x0000;
    uint8_t preset = 1;
    uint8_t screenSize = 4;
    CONFIG(const std::array<std::byte, 8> data);
    CONFIG();
    bool isConfigHeaderCorrect() const;
    bool isConfigValuesCorrect() const;
};

struct SCORES : public SAVE_FILE_BASE::CHUNK {
    uint32_t score1 = 0;
    uint32_t score2 = 0;
    uint32_t score3 = 0;
    char score1Name[6] = {'-', '-', '-', '-', '-', '-'};
    char score2Name[6] = {'-', '-', '-', '-', '-', '-'};
    char score3Name[6] = {'-', '-', '-', '-', '-', '-'};
    uint16_t padding = 0;
    SCORES(const std::array<std::byte, 32> data);
    SCORES();
    bool isScoresHeaderCorrect() const;
};

struct SAVE : public SAVE_FILE_BASE::SAVE {
    CONFIG config;
    SCORES scores;
    SAVE();
};

bool isDataIntegral(const std::vector<std::byte>& data);
void loadSave(const std::vector<std::byte>& data, SAVE_FILE_V0001::SAVE& save);
std::vector<std::byte> writeSave(const SAVE_FILE_V0001::SAVE& save);
}  // namespace SAVE_FILE_V0001