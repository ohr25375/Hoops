#include "sfxAssets.hpp"

const std::vector<std::vector<std::string>> sfxAssets {
    {
        // place
        "T180 O2 l64 P160 V11 b- V8 g O7 V6 c < V4 g d"
    },
    {
        // blip
        "T180 O5 P160 l32 V15 b"
    },
    {
        // select
        "T180 O5 P160 l32 V8 f+ V7 f+ V5 f+ V3 f+"
    },  
};

std::vector<std::string> getMML(const SFX_ASSET_ID& assetID) {
    auto id = (int)assetID;
    return sfxAssets[id];
}
