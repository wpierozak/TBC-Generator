#pragma once
#include<string>
#include <cstdint>
#include<random>
#include<iostream>
#include<cmath>
#include <functional> 

typedef int64_t _long_int;
typedef int32_t _int;

// Define the struct
struct cell
{
    uint16_t state;
    float time;

    cell(uint16_t s = __UINT16_MAX__, float t = 0): state(s), time(t)
    {

    }

    operator int() { return state; }
    // Implement equality comparison operator (necessary for unordered containers)
    bool operator==(const cell& other) const
    {
        return state == other.state && time == other.time;
    }
};


constexpr float PI_180 = M_PI/180.0;

inline float radians(float degrees) { return degrees * PI_180; }

struct f_vec
{
    float x,y,z;
    float norm() const {
        return sqrt(x*x + y*y + z*z);
    }

    f_vec operator*(float f) const
    {
        return {x*f, y*f, z*f};
    }

    float operator*(const f_vec& a) const
    {
        return x*a.x + y*a.y + z*a.z;
    }

    f_vec cross(const f_vec& b) const
    {
        return {y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x};
    }

    f_vec operator+(const f_vec& b) const
    {
        return {x + b.x, y + b.y, z + b.z};
    }

    f_vec operator-(const f_vec& b) const
    {
        return {x - b.x, y - b.y, z - b.z};
    }

    float cos(const f_vec& b) const
    {
        return ( (*this)*b  )/ (this->norm() * b.norm());
    }

    void normalize()
    {
        float norm = this->norm();
        x = x/norm;
        y = y/norm;
        z = z/norm;
    }
};

