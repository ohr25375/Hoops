#pragma once

#include <array>
#include <cstdint>
#include <cstddef>
#include <vector>

namespace SAVE_FILE_BASE {
constexpr uint32_t FOURCC(const char c1, const char c2, const char c3, const char c4);
constexpr uint32_t FOURCC(const std::array<char, 4> arr);
bool hasEnoughData(const size_t size);
bool isHeaderCorrect(const std::vector<std::byte>& data, const uint16_t version);
struct CHUNK {
private:
    uint32_t id;
    uint32_t chunkSize;
public:
    CHUNK(const std::array<char, 4> id, const uint32_t chunkSize);
    bool isChunkCorrect(const uint32_t id, const uint32_t size) const;
};
struct SAVE {
private:
    uint32_t id = SAVE_FILE_BASE::FOURCC({ 'S', 'A', 'V', 'E' });
    uint32_t fileSize;
    uint32_t fileID = SAVE_FILE_BASE::FOURCC({ 'R', 'I', 'N', 'G' });
    uint16_t version;
    uint16_t pad = 0;
public:
    uint32_t getFileSize() const { return fileSize; }
    SAVE(const uint32_t fileSize, const uint16_t version);
    bool isHeaderCorrect(const uint32_t size) const;
    bool isVersionCorrect(const uint16_t version) const;
};


constexpr uint32_t FOURCC(const char c1, const char c2, const char c3, const char c4) {
    return ((uint32_t)c1) |
           ((uint32_t)c2) << 8 |
           ((uint32_t)c3) << 16 |
           ((uint32_t)c4) << 24;
}
constexpr uint32_t FOURCC(const std::array<char, 4> arr) {
    return FOURCC(arr[0], arr[1], arr[2], arr[3]);
}
}  // namespace SAVE_FILE_BASE