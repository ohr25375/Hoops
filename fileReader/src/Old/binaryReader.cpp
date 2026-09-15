#include "binaryReader.hpp"

#include <fstream>
#include <iterator>
#include <iostream>
#include <iomanip>
#include "hexManip.hpp"

namespace binaryReader {
    std::vector<uint8_t> readBinary(std::filesystem::path path) {
        std::ifstream input(path, std::ios::binary);
    
        std::vector<uint8_t> data(
            (std::istreambuf_iterator<char>(input)),
            (std::istreambuf_iterator<char>())
        );
        input.close();
        return data;
    }
    
    void printBinary(std::vector<uint8_t> data) {
        for (int i = 0; i < (int)data.size(); i++) {
            if (i % 16 == 0) {
                std::cout << toHex(i, 8) << ": ";
            }
            std::cout << toHex(data[i], 2) << (i % 2 == 1 ? " " : "");
            if (i % 16 == 15) {
                std::cout << " ";
                for (int j = 15; j >= 0; j--) {
                    if ((int)data[i - j] >= 33 && (int)data[i - j] <= 126) {
                        std::cout << data[i - j];
                    } else {
                        std::cout << ".";
                    }
                }
                std::cout << '\n';
            }
        }
    }
}

