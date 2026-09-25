#include "colorPresets.hpp"

struct COLOR_PRESET {
    std::string name;
    std::array<SDL2Addon::SDL_COLOR, 2> palette;
    COLOR_PRESET(const std::string name, const std::array<SDL2Addon::SDL_COLOR, 2> colors) : name(name), palette(colors) {}
};

const std::vector<COLOR_PRESET> COLOR_PRESETS {
    COLOR_PRESET("USER", {0x000000, 0xffffff}),
    COLOR_PRESET("MONO", {0x000000, 0xffffff}),
    COLOR_PRESET("INVERT", {0xffffff, 0x000000}),
    COLOR_PRESET("COOL", {0x000011, 0xddeedd}),
    COLOR_PRESET("BLUE", {0x000011, 0x0088ff}),
    COLOR_PRESET("RED", {0xcc0000, 0xff9988}),
    COLOR_PRESET("GREEN", {0x55aa55, 0x006600}),
    COLOR_PRESET("ORANGE", {0xbb8811, 0xffdd99}),
    COLOR_PRESET("YE OLDE", {0x221100, 0xffdd99}),
    COLOR_PRESET("OLDE YE", {0xffdd99, 0x221100}),
    COLOR_PRESET("BSOD", {0x0088ff, 0xccccee}),
    COLOR_PRESET("CONSOLE", {0x000000, 0x44ff44}),
    COLOR_PRESET("NULL", {0xff00ff, 0x00ffff}),
};


std::array<SDL2Addon::SDL_COLOR, 2> getColorPreset(const int& index) {
    return COLOR_PRESETS[index % COLOR_PRESETS.size()].palette;
}

std::string getColorPresetName(const int& index) {
    return COLOR_PRESETS[index % COLOR_PRESETS.size()].name;
}

size_t getPresetSize() {
    return COLOR_PRESETS.size();
}