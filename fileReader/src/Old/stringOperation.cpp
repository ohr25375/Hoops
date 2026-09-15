#include "stringOperation.hpp"

void PRINTER::setOffset(const char c, int length) {
    offset = std::string(length, c);
    offsetSize = length;
}

int PRINTER::getOffset() {
    return offsetSize;
}

void PRINTER::setOffsetLength(int length) {
    offset = std::string(length, offset[0]);
    offsetSize = length;
}

void PRINTER::setEndOffset(const char c, int length) {
    wholeSize = length;
    separator = c;
}
