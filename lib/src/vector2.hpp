#pragma once
#ifndef vector2_hpp
#define vector2_hpp

#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <SDL.h>
const double DEG2RAD = M_PI / 180;
const double RAD2DEG = 180 / M_PI;

template<typename T>
    requires std::integral<T> || std::floating_point<T>
class VECTOR2 {
public:
    T x, y;

    VECTOR2() {
        x = 0;
        y = 0;
    }

    VECTOR2(const T& xy) {
        x = xy;
        y = xy;
    }
    
    VECTOR2(const T& x, const T& y) {
        this->x = x;
        this->y = y;
    }

    template<typename T2>
    VECTOR2(const VECTOR2<T2> xy) {
        this->x = xy.x;
        this->y = xy.y;
    }

    VECTOR2& operator+=(const VECTOR2& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    VECTOR2 operator+(const VECTOR2& rhs) const {
        VECTOR2<T> res = *this;
        res += rhs;
        return res;
    }

    VECTOR2& operator-=(const VECTOR2& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    VECTOR2 operator-(const VECTOR2& rhs) const {
        VECTOR2<T> res = *this;
        res -= rhs;
        return res;
    }

    VECTOR2 operator-() const {
        VECTOR2<T> res = *this;
        res.x = -res.x;
        res.y = -res.y;
        return res;
    }

    VECTOR2& operator/=(const T rhs) {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    VECTOR2 operator/(const T rhs) const {
        VECTOR2<T> res = *this;
        res /= rhs;
        return res;
    }

    VECTOR2& operator*=(const T rhs) {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    VECTOR2 operator*(const T rhs) const {
        VECTOR2<T> res = *this;
        res *= rhs;
        return res;
    }

    VECTOR2 mod(const VECTOR2<T> rhs) const {
        VECTOR2<T> temp;
        temp.x = fmod(x, rhs.x);
        temp.y = fmod(y, rhs.y);
        return temp;
    }

    VECTOR2 rotate2D(const T& rot) const {
        VECTOR2<T> res;
        res.x = this->x * cos(rot) - this->y * sin(rot);
        res.y = this->x * sin(rot) + this->y * cos(rot);
        return res;
    }

    VECTOR2 normalized() const {
        VECTOR2<T> res = *this;
        if (res.magnitude() == 0) {
            return res;
        }
        res /= res.magnitude();
        return res;
    }

    T dot(const VECTOR2& rhs) const {
        return x * rhs.x + y * rhs.y;
    }

    T magnitude() const {
        return sqrt(this->dot(*this));
    }

    double angle() const {
        return atan2(y, x);
    }

    double angle(const VECTOR2& rhs) const {
        return rhs.angle() - angle();
    }

    T equality(const VECTOR2& rhs) const {
        VECTOR2 temp1 = *this;
        VECTOR2 temp2 = rhs;
        return ((int)temp1.x == (int)temp2.x) && ((int)temp1.y == (int)temp2.y);
    }

    T equality(const VECTOR2& rhs, double theta) const {
        double distance = (*this - rhs).magnitude();
        return distance <= theta;
    }

    VECTOR2<int> sign() const {
        VECTOR2<int> res;
        res.x = x > 0 ? 1 : x < 0 ? -1 : 0;
        res.y = y > 0 ? 1 : y < 0 ? -1 : 0;
        return res;
    }

    template <typename T2>
    VECTOR2 scale(const VECTOR2<T2>& rhs) const {
        VECTOR2 res = *this;
        res.x *= rhs.x;
        res.y *= rhs.y;
        return res;
    }

    template <typename T2>
    VECTOR2 shrink(const VECTOR2<T2>& rhs) const {
        VECTOR2 res = *this;
        res.x /= rhs.x;
        res.y /= rhs.y;
        return res;
    }

    VECTOR2<int> round() const {
        VECTOR2<int> res;
        res.x = std::round(x);
        res.y = std::round(y);
        return res;
    }

    bool isFacing(const VECTOR2& other) const {
        return this->dot(other) < 0;
    }

    operator std::pair<T, T>() const {
        return {x, y};
    }

    operator SDL_Point() const {
        return {.x = (int)x, .y = (int)y};
    }

    operator SDL_FPoint() const {
        return {.x = (float)x, .y = (float)y};;
    }

    operator std::string() const {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }
};

template<typename T>
    requires std::integral<T> || std::floating_point<T>
std::ostream& operator<< (std::ostream& out, const VECTOR2<T> n)
{
    return out << std::fixed << std::setprecision(2) << "[" << n.x << "," << n.y << "]";
}

#define VECTOR2d VECTOR2<double>
#define VECTOR2i VECTOR2<int>
#define VECTOR2f VECTOR2<float>

#endif