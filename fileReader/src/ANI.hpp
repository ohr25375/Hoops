#pragma once
#ifndef ANI_FILE
#define ANI_FILE

#include "RIFF.hpp"
#include "CUR.hpp"

namespace ANI {
    // ACON Specifics
    // RIFF('ACON'                        
    //     [LIST('INFO'                   
    //         [INAM(<ZSTR>)]             // Title. Optional.
    //         [IART(<ZSTR>)]             // Author. Optional.
    //     )]                             
    //     'anih'(<ANIHEADER>)            // ANI file header.
    //     ['rate'(<DWORD...>)]           // Rate table (array of jiffies).
    //                                 // If the AF_SEQUENCE flag is set
    //                                 // then the count is ANIHEADER.cSteps,
    //                                 // otherwise ANIHEADER.cFrames.
    //     ['seq '(<DWORD...>)]           // Sequence table (array of frame index values).
    //                                 // Should be present when AF_SEQUENCE flag is set.
    //                                 // Count is ANIHEADER.cSteps.
    //     LIST('fram'                    // List of frames data. Count is ANIHEADER.cFrames.
    //     'icon'(<icon_data_1>)       // Frame 1
    //     'icon'(<icon_data_2>)       // Frame 2
    //     ...
    //     )
    // )
    const RIFF::CKID ANI_ID = 0x4e4f4341;
    const RIFF::CKID ANI_HEADER_ID = 0x68696e61;
    const RIFF::CKID ANI_SEQ_ID = 0x20716573;
    const RIFF::CKSIZE ANI_HEADER_SIZE = 36;
    const RIFF::CKID ANI_ICON_ID = 0x6e6f6369;
    const RIFF::CKSIZE ANI_ICON_MINIMAL_SIZE = 62;
    const RIFF::CKID ANI_FRAM_ID = 0x6d617266;
    const RIFF::CKSIZE ANI_FRAM_MINIMAL_SIZE = ANI_ICON_MINIMAL_SIZE + 12;
    const RIFF::CKID ANI_RATE_ID = 0x65746172;

    class ANI_HEADER : public RIFF::CK {
    public:
        RIFF::CKSIZE hdSize;
        RIFF::DWORD frameCount;
        RIFF::DWORD stepCount;
        RIFF::DWORD imageWidth;
        RIFF::DWORD imageHeight;
        RIFF::DWORD bitCount;
        RIFF::DWORD planeCount;
        RIFF::DWORD rate;
        RIFF::DWORD flags;

        ANI_HEADER();
        ANI_HEADER(RIFF::CKSIZE ckSize, std::vector<RIFF::BYTE>::iterator& data);
        virtual ~ANI_HEADER() = default;
        void print();
    private:
    };

    class ANI_ICON : public RIFF::CK {
    public:
        CUR::CUR cursorData;

        ANI_ICON();
        ANI_ICON(RIFF::CKSIZE ckSize, std::vector<RIFF::BYTE>::iterator& data);

        void print();
    private:
    };

    class ANI_FRAM : public RIFF::CK {
    public:
        std::vector<ANI_ICON> icons;

        ANI_FRAM();
        void insertFRAM(RIFF::CKSIZE ckSize, std::vector<RIFF::BYTE>::iterator& data);

        void print();
    private:
    };


    class ANI_SEQ : public RIFF::CK {
    public:
        std::vector<RIFF::DWORD> frameOrder;

        ANI_SEQ();
        virtual ~ANI_SEQ() = default;

        void insertSequence(RIFF::CKSIZE ckSize, std::vector<RIFF::BYTE>::iterator& data);
        void print();
    private:
    };

    class ANI_RATE : public RIFF::CK {
    public:
        std::vector<RIFF::DWORD> timing;

        ANI_RATE();
        virtual ~ANI_RATE() = default;

        void insertRate(RIFF::CKSIZE ckSize, std::vector<RIFF::BYTE>::iterator& data);
        void print();
    };

    class ANI {
    public:
        ANI_HEADER header;
        ANI_SEQ sequence;
        ANI_RATE rate;
        ANI_FRAM frames;

        ANI(std::vector<RIFF::BYTE> &data, bool debug = false);

        
        std::vector<RIFF::BYTE> unread;
        void print();
        private:
        void readACON(std::vector<RIFF::BYTE>::iterator &it, std::vector<RIFF::BYTE> &data);
    };
} // ANI

#endif