#pragma once

struct ubyte4x2
{
    char8_t l : 4;
    char8_t r : 4;
    ubyte4x2(const char8_t v);
    operator char() const;
    ubyte4x2& operator+=(const ubyte4x2 r);
};