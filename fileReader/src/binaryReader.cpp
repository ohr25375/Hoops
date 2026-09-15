#include "binaryReader.hpp"

#include <fstream>
#include <iterator>
#include <iostream>
#include "hexManipulator.hpp"

namespace binaryReader {
    // std::vector<std::byte> getBytesFromFile(std::filesystem::path path) {
    //     std::ifstream fstream(path, std::ios::binary);
    //     std::size_t size = std::filesystem::file_size(path);
    //     std::vector<std::byte> bytes(size);
    //     fstream.read(reinterpret_cast<char*>(bytes.data()), size);
    //     fstream.close();
    //     return bytes;
    // }
    std::vector<uint8_t> getBytesFromFile(std::filesystem::path path) {
        std::ifstream fstream(path, std::ios::binary);
        std::size_t size = std::filesystem::file_size(path);
        std::vector<uint8_t> bytes(size);
        fstream.read(reinterpret_cast<char*>(bytes.data()), size);
        fstream.close();
        return bytes;
    }

    void printBytes(const std::vector<std::byte>& bytes) {
        std::cout << "Size of bytes : " << bytes.size() << '\n';

        for (int32_t i = 0; i < (int32_t)bytes.size(); i++) {
            if (i % 16 == 0) {
                std::cout << hexManipulator::toHexString(i, 8) << ": ";
            }
            std::cout << hexManipulator::toHexString(bytes[i], 2) << (i % 2 == 1 ? " " : "");
            if (i % 16 == 15) {
                std::cout << " ";
                for (int32_t j = 15; j >= 0; j--) {
                    if (std::to_integer<uint8_t>(bytes[i - j]) >= 33 && std::to_integer<uint8_t>(bytes[i - j]) <= 126) {
                        std::cout << std::to_integer<uint8_t>(bytes[i - j]);
                    } else {
                        std::cout << ".";
                    }
                }
                std::cout << '\n';
            }
        }
    }
    void printBytes(const std::vector<uint8_t>& bytes) {
        std::cout << "Size of bytes : " << bytes.size() << '\n';

        for (int32_t i = 0; i < (int32_t)bytes.size(); i++) {
            if (i % 16 == 0) {
                std::cout << hexManipulator::toHexString(i, 8) << ": ";
            }
            std::cout << hexManipulator::toHexString(bytes[i], 2) << (i % 2 == 1 ? " " : "");
            if (i % 16 == 15) {
                std::cout << " ";
                for (int32_t j = 15; j >= 0; j--) {
                    if (bytes[i - j] >= 33 && bytes[i - j] <= 126) {
                        std::cout << bytes[i - j];
                    } else {
                        std::cout << ".";
                    }
                }
                std::cout << '\n';
            }
        }
        if (bytes.size() % 16 != 0) {
            std::cout << '\n';
        }
    }
} // binaryReader