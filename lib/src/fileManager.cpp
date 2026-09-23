#include "fileManager.hpp"

bool readBinary(const std::string& path, std::vector<std::byte>& out) {
    std::basic_ifstream<char> file(path, std::ios::binary);
    if (!file) return false;

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    out = std::vector<std::byte>(fileSize);
    file.read((char*)&out[0], fileSize);
    return true;
}

void writeBinary(const std::string& path, const std::vector<std::byte>& data) {
    std::basic_ofstream<char> fs(path, std::ios::binary);
    fs.write(reinterpret_cast<const char*>(data.data()), sizeof(data[0]) * data.size());
    fs.flush();
    fs.close();
}