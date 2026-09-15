#include "bgmAssets.hpp"

const std::vector<std::vector<std::string>> audioAssets {
    std::vector<std::string> {},
    std::vector<std::string> {
        // rings
        "T140 O4 L8 V15 I0 P120"
        "f+b>c+"
        "d4v6d8v15dl16c+v6c+V15l24c+dc+l8<ba"
        "b>f+16r16<b->a16r16f+.V8f+16V15ef+"
        "g<l16b-rb-8>g8f+8<brl8b>f+"
        "el24dedl8c+<bl16b-rl8f+b>c+"

        "d4v6d8v15dl16c+v6c+V15l24c+dc+l8<ba"
        "b>f+16r16<b->a16r16f+.V8f+16V15ef+"
        "g<l16b-rb-8>g8f+8<brl8b>f+"
        "l16ev8ev15l24ef+el8dc+<bv8b>v15ef+"

        "l16gv8gv15gf+l8gab4ag"
        "l16f+v8f+v15f+el8def+v8f+v15c+d"
        "ev8ev15f+gf+edc+"
        "l16<br>c+rdrerf+rl8d+ef+"

        "l16g v8 g v15gf+ l8 g a b l16 v8b v15b>c+r<b8"
        "l16f+v8f+v15f+el8def+v8f+v15c+d"
        "ev8ev15f+gf+edc+"
        "<b16r.b-4l16br",

        "T140 O4 L8 V15 I0 P120"
        "dc+<a"
        "b4v6b8v15bl16av6aV15l24abal16f+v8f+v15f+v8f+"
        "v15l8f+>d16r16<f+>f+16r16d.V8d16V15c+d"
        "e<l16grg8>e8d8<f+rl8f+>d"
        "c+l24c-c+<bl8b-gl16f+r>l8dc+<a"

        "b4v6b8v15bl16av6aV15l24abal16f+v8f+v15f+v8f+"
        "v15l8f+>d16r16<f+>f+16r16d.V8d16V15c+d"
        "e<l16grg8>e8d8<f+rl8f+>d"
        "l16c+v8c+v15l24c+dc+<l8bb-bv8bv15>l8ga"

        "l16bv8bv15bb-l8b>c+d4c+<b"
        "l16bv8bv15bgl8f+gav8av15l16f+v8f+v15f+v8f+"
        "l8v15f+v8f+v15b-bb-gf+e"
        "l16drerf+rgrarf+8gra8"

        "l16b v8 b v15bb- l8 b >c+ d l16r v8d v15erd8<"
        "l16av8av15agl8f+gf+v8f+ v15l16ev8e v15f+v8f+"
        "l8v15gv8gv15abb-gf+e"
        "d16r.c+4l16c-r",

        "T140 O2 L8 V15 I1 P120"
        "rf+4"
        "[b>b<]2[a>a<]2[g>g<]2[b>b<]2"
        "[b->b-<]2[b>b<]2>[c+>c+<]<2[f+>f+<]2"
        "[b>b<]2[a>a<]2[g>g<]2[b>b<]2"
        "[b->b-<]2[b>b<]2[f+>f+<]2[b>b<]2"

        "[e>e<]4[b>b<]4"
        "[f+>f+<]4[b>b<]2[f+>f+<]2"
        "[e>e<]4[b>b<]4"
        "[f+>f+<]4brf+4b"
    },
    std::vector<std::string> {
        // alloy
        ""
    },
    std::vector<std::string> {
        // place
        "T180 O2 l64 P160 V11 b- V8 g O7 V6 c < V4 g d"
    },
};

std::vector<std::string> getMML(const AUDIO_ASSET_ID& assetID) {
    return audioAssets[assetID];
}