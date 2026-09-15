#pragma once

#include <string>
#include <iostream>

class PRINTER {
public:
    void setOffset(const char c, int length);
    int getOffset();
    void setOffsetLength(int length);
    void setEndOffset(const char c, int length);
    template <typename T>
    void print(std::string text, T variable) {
        int trailingCount = wholeSize - text.size();
        if (trailingCount < 0) {
            wholeSize += abs(trailingCount);
            trailingCount = 0;
        }
        std::string trail(trailingCount + 1, ' ');
        std::cout 
            << offset 
            << text 
            << trail
            << separator
            << ' '
            << variable 
            << '\n';
    }
private:
    std::string offset;
    int offsetSize;
    int wholeSize = 16;
    char separator = ':';
};