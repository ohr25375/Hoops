#include "ICOReader.hpp"

#include <iostream>

#include "hexManip.hpp"

#define endl '\n'

void decodeICO(std::vector<uint8_t>::iterator &it, int depth) {
    std::string depthOffset(depth, '-');
    
    int count = toIntRev(it, 2);
    std::cout << depthOffset << "Type : " << count << endl;

    
}