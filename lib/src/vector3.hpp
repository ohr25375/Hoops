#pragma once
#ifndef vector3_hpp
#define vector3_hpp

template<typename T>
    requires std::integral<T> || std::floating_point<T>
class VECTOR3 {
public:
    T x, y, z;

    VECTOR3() {
        x = 0;
        y = 0;
        z = 0;
    }

    VECTOR3(const T& xyz) {
        this->x = xyz;
        this->y = xyz;
        this->z = xyz;
    }

    template<typename T2>
    VECTOR3(const VECTOR3<T2> xyz) {
        this->x = xyz.x;
        this->y = xyz.y;
        this->z = xyz.z;
    }
    
    VECTOR3(const T& x, const T& y, const T& z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    VECTOR3& operator+=(const VECTOR3& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    VECTOR3 operator+(const VECTOR3& rhs) const {
        VECTOR3<T> res = *this;
        res += rhs;
        return res;
    }

    VECTOR3& operator-=(const VECTOR3& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    VECTOR3 operator-(const VECTOR3& rhs) const {
        VECTOR3<T> res = *this;
        res -= rhs;
        return res;
    }

    VECTOR3& operator/=(const T rhs) {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }

    VECTOR3 operator/(const T rhs) const {
        VECTOR3<T> res = *this;
        res /= rhs;
        return res;
    }

    VECTOR3& operator*=(const T rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    VECTOR3 operator*(const T rhs) const {
        VECTOR3<T> res = *this;
        res *= rhs;
        return res;
    }

    VECTOR3 mod(const VECTOR3<T> rhs) const {
        VECTOR3<T> temp;
        temp.x = fmod(x, rhs.x);
        temp.y = fmod(y, rhs.y);
        temp.z = fmod(z, rhs.z);
        return temp;
    }

    VECTOR3 rotate2D(const T& rot) const {
        VECTOR3<T> res;
        res.x = this->x * cos(rot) - this->y * sin(rot);
        res.y = this->x * sin(rot) + this->y * cos(rot);
        res.z = 0;
        return res;
    }

    VECTOR3 rotateX(const T& rot) const {
        VECTOR3<T> res;
        res.x = this->x;
        res.y = this->y * cos(rot) - this->z * sin(rot);
        res.z = this->y * sin(rot) + this->z * cos(rot);
        return res;
    }

    VECTOR3 rotateY(const T& rot) const {
        VECTOR3<T> res;
        res.x = this->z * sin(rot) + this->x * cos(rot);
        res.y = this->y;
        res.z = this->z * cos(rot) - this->x * sin(rot);
        return res;
    }

    VECTOR3 rotateZ(const T& rot) const {
        VECTOR3<T> res;
        res.x = this->x * cos(rot) - this->y * sin(rot);
        res.y = this->x * sin(rot) + this->y * cos(rot);
        res.z = 0;
        return res;
    }

    VECTOR3 normalized() const {
        VECTOR3<T> res = *this;
        res /= res.magnitude();
        return res;
    }

    T dot(const VECTOR3& rhs) const {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    T magnitude() const {
        return sqrt(this->dot(*this));
    }

    bool isFacing(const VECTOR3& other) const {
        return this->dot(other) < 0;
    }
};

template<typename T>
    requires std::integral<T> || std::floating_point<T>
std::ostream& operator<< (std::ostream& out, const VECTOR3<T> n)
{
    return out << "[" << n.x << "," << n.y << "," << n.z << "]";
}

#define VECTOR3d VECTOR3<double>
#define VECTOR3i VECTOR3<int>
#define VECTOR3f VECTOR3<float>

#endif