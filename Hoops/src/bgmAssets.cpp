#include "bgmAssets.hpp"

struct AUDIO_ASSET {
    std::string name;
    std::vector<std::string> mml;
    AUDIO_ASSET(const std::string& name, const std::vector<std::string>& mml) : name(name), mml(mml) {}
};

const std::vector<AUDIO_ASSET> audioAssets {
    AUDIO_ASSET("NONE", std::vector<std::string>(0)),
    AUDIO_ASSET("RINGS", std::vector<std::string>{
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
    }),
    AUDIO_ASSET("ALLOY", std::vector<std::string>{
        // alloy
        "T180 O2 l24 P130 V15"
        "[<fr>frfr [fr]9 frgra-r b-r>crd-r crr6<a-rr6"
        "<fr>[fr]11 frgra-r b-r>crd-r crr6<a-rr6"
        "<fr>frfr [fr]9 <gr>[gr]11 <a-r>[a-r]5 <er>[er]5]3"
        "<fr>frfr [fr]9 frgra-r b-r>crd-r crr6<a-rr6"
        "<b-r>b-rb-r [b-r]9 <fr>frfr [fr]3 frgra-ra-rgrfr"
        "<b-r>b-rb-r [b-r]9 <fr>frfr [fr]3 frgra-ra-rgrfr"
        "<b-r>b-rb-r [b-r]9 <fr>frfr [fr]3 frgra-ra-rgrfr"
        "<gr>grgr [gr]9 <cr>crcr [cr]3 > cr<b-ra-rb-ra-rgr"
        ,
        "T180 O2 l24 P128 V15 !1"
        "[[f12.rfr]8"
        "[f12.rfr]8"
        "[f12.rfr]4 <[b-12.rb-r]3 b12.rbr"
        ">[c12.rcr]4]3"
        "[f12.rfr]8"
        "<[b-12.rb-r]4>[f12.rfr]4"
        "<[b-12.rb-r]4>[f12.rfr]4"
        "<[b-12.rb-r]4>[f12.rfr]4"
        "[g12.rgr]4[c12.rcr]4"
        ,
        "T180 O4 l4 P124 V15 !2"
        "[r1]7"
        "r1"
        "[l6 r3. r. c v8 c12"
        "v15 f v8 f12 v15 g v8 g12 v15 a- v8 a-12 v15 b- v8 b-12 v15 >c v8 c12 v15 <a- v8 a-12 v15 f v8 f12 r."
        "v15 r3. r. >c+ v8 c+12 v15 c v8 c12 v15 <a- v8 a-12 v15 f8.r16 l12frg"
        "l3 a-.e."
        "f.r.]2"
        "r3. r6."
        "l12>crf"
        "l6d-3.<b-.b.b+3.f.>l12fre-"
        "l6d-3.<b-.b.b+3.r.>l12frf"
        "l6d-3.<b+.b-.b+.a-.f.l12frg"
        "l3a-.f.b+.r."
        ,
        "T180 O3 l4 P124 V15 !2"
        "[r1]7"
        "[r1]7"
        "r1"
        "l4r2. a-"
        ">cefga-fcr"
        "r2.b-a-fc8.r16l12cre"
        "f2c2"
        "<a-2r2"
        "r2."
        "l12>grb+"
        "l4b-2g8.r16ga-2cl12>d-rc<"
        "l4b-2g8.r16ga-2rl12>d-rd-<"
        "l4b-2g8.r16ga-fcl12cre"
        "l2fcgr"
    }),
    AUDIO_ASSET("ERROR", std::vector<std::string>(0)),
    AUDIO_ASSET("ODE", std::vector<std::string>{
        "T160 O5 l16 V15 !2"
        "ar8. ar8. b-r8. b+r8. b+r8. b-r8. ar8. gr8."
        "fr8. fr8. gr8. ar8. ar8. gr8. gr8. r4"
        "ar8. ar8. b-r8. b+r8. b+r8. b-r8. ar8. gr8."
        "fr8. fr8. gr8. ar8. gr8. fr8. fr8. r4",
        "T160 O5 l16 V15 !2 P123"
        "[r8cf]4 [r8ce]4"
        "[r8cf]4 [r8ce]3 erce"
        "[r8cf]4 [r8ce]4"
        "[r8cf]4 r8ce r8cf r8cf frcf",
        "T160 O3 l16 V15 P120"
        "f4frfr fr8.fr8. c4crcr crcrdrer"
        "f8.rcrfr frcrfrfr c8.rcrcr crcrdrer"
        "f4frfr fr8.fr8. c4crcr crcrdrer"
        "f8.rcrfr frcrfrfr crcrdrer frcrf8.r",
        "T160 O5 l64 V15 I2"
        "[cr16.rcr32.cr32.]32"
    }),
    AUDIO_ASSET("HOOPS", std::vector<std::string>{
        "T120 O5 l16 V15 !2"
        "fe-de-"
        "frfrgrgr c4dre-r de-d<b->crcr d8.rfe-de-"
        "frfrgrgr c4dre-r de-d<b->crcr <b-8.r>"

        "frb-r"
        "ararb-rb-r b+4frb-r ararb-rb+r f8.rgrar"
        "b-8argrdr f8e-rcrdr"

        "l8 O4"
        "b-4fb- b+4f>c e-dc<b-> d4c4"
        "<b-4>b-a gfd16c16<b-> e-de-fd4c4<"
        "b-4fb- b+4f>c e-dc<b-> d4c4"
        "<b-4>b-a gfd16c16<b-> e-de-fd4c4"

        "l32<"
        "b-4r8b-rb-r> c+4r8c+rc+r e-4r8e-re-r g+4",

        "T120 O5 l16 V15 !1"
        "r4"
        "drdre-re-r <a4b-rb+r b-8agarar b-8frdrb-8>"
        "drdre-re-r <a4b-rb+r b-8agarar b-fdc<b-4>>"
        "crcrdrdr e-8cr<ar>dr crcrdre-r <a8frb-r>cr"
        "d8cr<b-rfr a8gre-rfr<"

        "l8"
        "b-f>d<f b+f>e-<f gd>d<d f>d16c16<b-a"
        "gdb-d fdad e-<b->g<b-> dfab+"
        "gdb-d adb+d b-e-ge- f>d16c16<b-a"
        "gdb-d fdad e-<b->g<b-> dfab+"

        "l32"
        ">[d16r16drdr]2 [f+16r16f+rf+r]2"
        "[g+16r16g+rg+r]2> c+4",

        "T120 O2 l16 V15 !2"
        "r4"
        "b-8.rb-rb-r f8.rfrfr e-8.rfrfr b-8.rb-rb-r"
        "b-8.rb-rb-r f8.rfrfr e-8.rfrfr b-4<b-4>"
        "[f8.rfrfr]4"
        "e-8.re-re-r c8.rfrfr"

        "l2"
        "<b->fgb-4a4"
        "gde-f4<f4>"
        "gde-b-4a4"
        "gde-f"

        "l32"
        "b-16r16b-rb-rb-4"
        "[r8b-rb-rb-4]2"
        "f4",

        "T120 l64 V15 I2"
        "r4"
        "[crr16.cr32.cr32. crr16.crr16.]7 crr16.cr32.cr32.crr16.r8"
        "[crr16.cr32.cr32. crr16.crr16.]6"

        "crr16.crr16.[r8crr16.]5 r8cr32.cr32.crr16.crr16."
        "[[r8crr16.]6 r8cr32.cr32.crr16.crr16.]2"
        "[r8crr16.]6 r8cr32.cr32.cr32.cr32.crr16."
        
        "[r8cr32.cr32.crr16.r8]3 r8cr32.cr32."
    }),
};

std::vector<std::string> getMML(const AUDIO_ASSET_ID& assetID) {
    auto id = (int)assetID;
    return audioAssets[id].mml;
}

std::string getAudioAssetName(const AUDIO_ASSET_ID& assetID) {
    return audioAssets[(int)assetID].name;
}
