#pragma once

#include <cstdint>

typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef uint8_t BYTE;

#define NEGATIVE_BIT (0b10000000)
#define BYTE_MASK (0xff)
#define MAX_BYTE (0x100)
#define WORD_MASK (0xffff)
#define MAX_WORD (0x10000)
#define DWORD_MASK (0xffffffff)
#define MAX_DWORD (0x100000000)

struct NIBBLE {
    BYTE lower : 4;
    BYTE upper : 4;
};

union UNIBBLE {
    BYTE byte;
    NIBBLE nibble;
};