#include "saveFile_v0001.hpp"

#include <vector>
#include <iostream>

namespace SAVE_FILE_V0001 {
    const int VOLUME_MAX = 5;
    const int TRACK_MAX = 3;
    const int PRESET_MAX = 13;
    const int SCREEN_SIZE_MAX = 11;

CONFIG::CONFIG(const std::array<std::byte, 8> data)
    : CHUNK({ 'C', 'O', 'N', 'F' }, data.size()),
      volume(std::to_integer<uint8_t>(data[0])),
      track(std::to_integer<uint8_t>(data[1])),
      colorA(std::to_integer<uint16_t>(data[2])),
      colorB(std::to_integer<uint16_t>(data[4])),
      preset(std::to_integer<uint8_t>(data[6])),
      screenSize(std::to_integer<uint8_t>(data[7])) {}
CONFIG::CONFIG()
    : CHUNK({ 'C', 'O', 'N', 'F' }, 8) {}
bool CONFIG::isConfigHeaderCorrect() const {
    return isChunkCorrect(SAVE_FILE_BASE::FOURCC({ 'C', 'O', 'N', 'F' }), 8);
}
bool CONFIG::isConfigValuesCorrect() const {
    return !(
        (volume & 0xf) >= VOLUME_MAX ||
        (volume >> 4 & 0xf) >= VOLUME_MAX ||
        track >= TRACK_MAX ||
        (colorA >> 12 & 0xf) != 0 ||
        (colorB >> 12 & 0xf) != 0 ||
        preset >= PRESET_MAX ||
        screenSize == 0 || screenSize >= SCREEN_SIZE_MAX
    );
}
SCORES::SCORES(const std::array<std::byte, 24> data)
    : CHUNK({ 'S', 'C', 'O', 'R' }, data.size()),
      score1(std::to_integer<uint16_t>(data[0])),
      score2(std::to_integer<uint16_t>(data[8])),
      score3(std::to_integer<uint16_t>(data[16])) {
    for (auto i = 0; i < 6; i++) {
        score1Name[i] = std::to_integer<char>(data[1 + i]);
        score2Name[i] = std::to_integer<char>(data[9 + i]);
        score3Name[i] = std::to_integer<char>(data[9 + i]);
    }
}
SCORES::SCORES()
    : CHUNK({ 'S', 'C', 'O', 'R' }, 24) {}
bool SCORES::isScoresHeaderCorrect() const {
    return isChunkCorrect(SAVE_FILE_BASE::FOURCC({ 'S', 'C', 'O', 'R' }), 24);
}
SAVE::SAVE()
    : SAVE_FILE_BASE::SAVE(56, 1) {}
bool checkSize(const size_t size) {
    return size == 64;
}
bool isDataIntegral(const std::vector<std::byte>& data) {
    if (!SAVE_FILE_BASE::isHeaderCorrect(data, 1)) return false;
    // check has enough data for V0001 save file
    if (!checkSize(data.size())) return false;
    auto testData = reinterpret_cast<const SAVE_FILE_V0001::SAVE*>(data.data());
    if (!testData->config.isConfigHeaderCorrect()) return false;
    if (!testData->config.isConfigValuesCorrect()) return false;
    if (!testData->scores.isScoresHeaderCorrect()) return false;
    return true;
}
void loadSave(const std::vector<std::byte>& data, SAVE_FILE_V0001::SAVE& save) {
    if (!SAVE_FILE_V0001::isDataIntegral(data)) {
        std::cout << "is corrupt\n";
    } else {
        std::cout << "correct data\n";
        auto loadedSave = reinterpret_cast<const SAVE_FILE_V0001::SAVE*>(data.data());
        save = *loadedSave;
    }
}
std::vector<std::byte> writeSave(const SAVE_FILE_V0001::SAVE& save) {
    auto bytes = reinterpret_cast<const std::byte*>(&save);
    return std::vector<std::byte>(bytes, bytes + (save.getFileSize() + 8));
}
}  // namespace SAVE_FILE_V0001