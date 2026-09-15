#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <set>
#include <deque>
#include <map>

namespace MML_Internal {
    struct ChannelAttributes;

    int32_t getNumber(std::string::const_iterator& it, std::string::const_iterator& end, uint32_t maxValue, uint32_t& value, bool required = true, uint32_t defaultValue = 0);
    int16_t generateSample(ChannelAttributes& channelAttributes);
    int16_t generateSine(ChannelAttributes& channelAttributes);
    int16_t generateSquare(ChannelAttributes& channelAttributes);
    int16_t generateTriangle(ChannelAttributes& channelAttributes);
    int16_t generateNoise(ChannelAttributes& channelAttributes);

    struct InstrumentAttributes {
        double baseFrequency = 440;
        double frequency     = 440;
        double baseVolume    = 0.8;
        double volume        = 0.8;
        long currentSample   = 0;
        long expectedSampleCount = 0;
        double angle         = 0;
    };
    struct ChannelAttributes {
        std::string mml      = "";
        uint16_t sampleRate  = 48000;
        uint16_t tempo       = 120;
        uint8_t length       = 4;
        uint8_t octave       = 4;
        uint8_t note         = 'a';
        int8_t noteFreqMod   = 0;
        uint8_t noteLength   = 4;
        uint8_t instMod      = 2;
        int8_t transpose     = 0;
        uint8_t fineTune     = 0x80;
        uint16_t volume      = 10;
        uint16_t volumeRange = 15;
        double pan           = 0.5;
        int16_t(*instrumentGenerator)(ChannelAttributes&) = generateSquare;
        InstrumentAttributes instrumentAttributes;
        std::string::const_iterator currentIndex;
        std::deque<std::string::const_iterator> repeatStarts;
        std::map<std::string::const_iterator, uint8_t> repeats;
    };
}

namespace MML {
    struct REPEATS;

    const char MML_TEMPO_SYMBOL = 't';
    const uint32_t MML_TEMPO_MAXVALUE = 999;
    const char MML_VOLUME_SYMBOL = 'v';
    const char MML_OCTAVE_SYMBOL = 'o';
    const char MML_OCTAVE_ADD_SYMBOL = '>';
    const char MML_OCTAVE_SUB_SYMBOL = '<';
    const uint32_t MML_OCTAVE_MAXVALUE = 10;
    const char MML_LENGTH_SYMBOL = 'l';
    const uint32_t MML_LENGTH_MAXVALUE = 64;
    const char MML_INSTRUMENT_SYMBOL = 'i';
    const uint32_t MML_INSTRUMENT_MAXVALUE = 2;
    const char MML_INSTRUMENTMOD_SYMBOL = '!';
    const uint32_t MML_INSTRUMENTMOD_MAXVALUE = 3;
    const char MML_FINEPITCH_SYMBOL = 'p';
    const uint32_t MML_FINEPITCH_MAXVALUE = 0xff;


    const char MML_REPEAT_START_SYMBOL = '[';
    const char MML_REPEAT_END_SYMBOL = ']';
    const uint32_t MML_REPEAT_MAXVALUE = 999;
    
    const double MML_A0 = 27.5;
    const int32_t MML_NOTE_OFFSET[] = { 0, 2, -9, -7, -5, -4, -2};
    const char MML_NOTE_ADD_SYMBOL = '+';
    const char MML_NOTE_SUB_SYMBOL = '-';
    const char MML_NOTE_DOT_SYMBOL = '.';
    const char MML_NOTE_REST_SYMBOL = 'r';

    const std::set<char> MML_VALID_CHARACTERS = {
        'a','b','c','d','e','f','g',
        MML_NOTE_ADD_SYMBOL,
        MML_NOTE_SUB_SYMBOL,
        MML_NOTE_DOT_SYMBOL,
        MML_NOTE_REST_SYMBOL,
        MML_TEMPO_SYMBOL,
        MML_VOLUME_SYMBOL,
        MML_OCTAVE_SYMBOL,
        MML_OCTAVE_ADD_SYMBOL,
        MML_OCTAVE_SUB_SYMBOL,
        MML_LENGTH_SYMBOL,

        MML_REPEAT_START_SYMBOL,
        MML_REPEAT_END_SYMBOL,

        MML_INSTRUMENT_SYMBOL,
        MML_INSTRUMENTMOD_SYMBOL,
    };

    class MML_Attributes {
    public:
        uint32_t tempo = 120;
        uint32_t length = 4;
        uint32_t octave = 4;
        double volume = 1;
        double angle = 0;
        double pan = 0.5;
    private:
    };

    class MML_RenderSettings {
    public:
        double volume = 1;
        int32_t transpose = 0;
        int32_t fineTune = 0;
        uint32_t globalTempo = 120;
    private:
    };

    class MML_Renderer {
    public:
        uint32_t maxVolume = 15;
        uint32_t sampleRate = 44100;
        bool isStereo = false;
        MML_RenderSettings renderSettings;

        std::vector<std::string> mml;

        std::vector<std::vector<uint8_t>> render() const;
        void setBitRate(int32_t bitRate);

    private:
        int32_t maxBitValue;
        int32_t bitRate;

        void pack(std::vector<std::vector<int32_t>> &track, std::vector<std::vector<uint8_t>> &stream) const;
        void scaleVolume(std::vector<std::vector<int32_t>> &track) const;
        std::vector<std::vector<int32_t>> compactInstruments(std::vector<std::vector<std::vector<int32_t>>> &insts) const;
        std::vector<std::vector<int32_t>> renderInstrument(uint32_t inst) const;
        void SampleNotes(char c, MML::MML_Attributes &attributes, std::string::const_iterator &it, std::string::const_iterator& end, int32_t &error, std::vector<std::vector<int32_t>> &stream) const;
        void DoRepeatEndSymbol(const std::string::const_iterator &begin, std::string::const_iterator &it, REPEATS &repeats, std::string::const_iterator &end, int32_t &error) const;
        
        std::vector<std::vector<int32_t>> readNote(char c, MML_Attributes &attributes, std::string::const_iterator &it, std::string::const_iterator& end, int32_t &error) const;
        std::vector<std::vector<int32_t>> readRest(const MML_Attributes& attributes, std::string::const_iterator &it, std::string::const_iterator& end, int32_t &error) const;
        void createSoundData(double frequency, MML::MML_Attributes &attributes, double dur, std::vector<int32_t> &soundData, int32_t stereoPan = 0) const;
    };
    
} // MML