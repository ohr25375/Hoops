#include "ubyte.hpp"

ubyte4x2::ubyte4x2(const char8_t v)
{
    l = (v & 0xf0) >> 4;
    r = (v & 0x0f);
}

ubyte4x2::operator char() const
{
    return (l << 4) + r;
}

ubyte4x2& ubyte4x2::operator+=(const ubyte4x2 r)
{
    *this = (char)*this + r;
    return *this;
}