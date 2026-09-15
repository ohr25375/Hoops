#pragma once

#include <array>
#include <filesystem>

namespace TON {
    struct TONE {
        std::array<bool, 256> tone;
        TONE() {tone.fill(false);}
    };
    
    
    class LOADER {
    public:
        std::array<TONE, 16> loadTone(const std::filesystem::path& path) const;
    };
}