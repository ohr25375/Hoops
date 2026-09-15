#include "TON.hpp"

#include <iostream>
#include <fstream>

namespace TON {
    std::array<TONE, 16> LOADER::loadTone(const std::filesystem::path& path) const {
        std::array<TONE, 16> res;
        res.fill(TONE());
        std::ifstream file(path);
        std::string line;
        auto toneIndex = 0;
        auto index = 0;
        std::cout << "TONLOADER: reading file...";
        while (std::getline(file, line)) {
            if (line[0] == ';') continue;
            if (line[0] == '\n') continue;
            if (line[0] == '\r') continue;
            toneIndex = index >> 8;
            for (auto i = 0; i < 16; i++) {
                res[toneIndex].tone[index & 0xff] = line[i] == '*';
                index++;
            }
        }
        std::cout << " done\n";
        return res;
    }
}