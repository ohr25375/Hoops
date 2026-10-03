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
    {
        // chain
        "T180 O6 V8 l32 [b- > f<]5"
    },
    {
        // spin
        "T240 O5 l32 V6 P160 g b-> d"
    },
    {
        // move
        "T180 O6 l32 V6 P160 f"
    },
    {
        // hard drop
        "T180 O7 l64 P160 V11 b- V8 f < V5 b"
    },
    {
        // game over
        "T180 O2 v15 l32cr l64[bfr32]7",
        "T180 !1 O2 v15 l32br l64[b-er32]7",
        "T180 !2 O2 v15 l32 b-r l64 [ae-r32]7"
    }
};

std::vector<std::string> getMML(const SFX_ASSET_ID& assetID) {
    auto id = (int)assetID;
    return sfxAssets[id];
}
