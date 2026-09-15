#pragma once
#ifndef keypress_hpp
#define keypress_hpp
#include <SDL2/SDL.h>

#include <map>

class KEYPRESS;

class KEYS {
public:
    KEYS();
    void update();
    KEYPRESS operator[](long n) const;
    KEYPRESS& operator[](long n);

private:
    std::map<long, KEYPRESS> keys;
};

class KEYPRESS {
public:
    /// @brief true on key down frame
    bool down;
    /// @brief true on key up frame
    bool up;
    /// @brief true while key down except 1st frame
    bool isHold;
    bool state;
    KEYPRESS();
    void update();

private:
    bool lastState;
};

class MOUSE {
public:
    int x, y;
    double xdir, ydir;
    double mouseSensitivity = 1.0;
    KEYPRESS left;
    KEYPRESS right;
    KEYPRESS middle;
    void update();

private:
    int xd, yd;
};

void updateKeys(std::map<int, KEYPRESS>& keys);
bool isAnyKeyDown(std::map<int, KEYPRESS>& keys);

#endif