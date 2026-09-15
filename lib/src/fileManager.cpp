#include "fileManager.hpp"

std::vector<char> readBinary(const std::string path)
{
    std::vector<char> vec(0);
    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        std::cout << "failed to open file" << '\n';
        return vec;
    }
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    vec = std::vector<char>(fileSize);
    file.read((char*)&vec[0], fileSize);
    return vec;
}