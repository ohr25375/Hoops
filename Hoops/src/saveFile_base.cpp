#include "saveFile_base.hpp"

#include <iostream>

namespace SAVE_FILE_BASE {
CHUNK::CHUNK(const std::array<char, 4> id, const uint32_t chunkSize)
    : id(FOURCC(id)), chunkSize(chunkSize) {}
bool CHUNK::isChunkCorrect(const uint32_t id, const uint32_t size) const {
    return !(this->id != id || chunkSize != size);
}
SAVE::SAVE(const uint32_t fileSize, const uint16_t version)
    : fileSize(fileSize), version(version) {}
bool hasEnoughData(const size_t size) {
    return size >= 16;
}
bool isHeaderCorrect(const std::vector<std::byte>& data, const uint16_t version) {
    // check has enough data for header
    if (!hasEnoughData(data.size())) return false;
    auto fileHeader_p = reinterpret_cast<const SAVE_FILE_BASE::SAVE*>(data.data());
    if (!fileHeader_p->isHeaderCorrect(data.size() - 8)) return false;
    // check version (incorrect version at V0001 will return corrupt data)
    if (!fileHeader_p->isVersionCorrect(version)) return false;
    return true;
}
bool SAVE::isHeaderCorrect(const uint32_t size) const {
    return !(fileSize != size || id != FOURCC({'S', 'A', 'V', 'E'}) || fileID != FOURCC({'R', 'I', 'N', 'G'}));
}
bool SAVE::isVersionCorrect(const uint16_t version) const {
    return this->version == version;
}
}  // namespace SAVE_FILE_BASE