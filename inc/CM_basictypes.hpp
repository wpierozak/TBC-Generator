#pragma once
#include<string>
#include <cstdint>
#include<random>
#include<cmath>

typedef int32_t cm_pos;
typedef int64_t cm_int;
typedef uint16_t cm_state;
typedef uint8_t cm_colorampl;
typedef uint8_t cm_smallsize;
typedef size_t cm_size;

const std::string TOP_SECTION = "top";
const std::string SMOOTH_SECTION = "smooth";
const std::string FEATHERED_SECTION = "feathered";

constexpr double PI_180 = M_PI/180.0;

inline double radians(double degrees) { return degrees * PI_180; }

struct cm_pos_vec
{
    cm_pos x,y,z;

    double norm() const {
        return sqrt(x*x + y*y + z*z);
    }
};

struct f_vec
{
    double x,y,z;
    double norm() const {
        return sqrt(x*x + y*y + z*z);
    }

    f_vec operator*(double f) const
    {
        return {x*f, y*f, z*f};
    }
};

void normalize(f_vec& vec);

inline double dotProduct(const f_vec& a, const f_vec& b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

inline f_vec crossProduct(const f_vec& a, const f_vec& b)
{
    return {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
}

inline double cos(const f_vec& a, const f_vec& b)
{
    return (a.x*b.x + a.y*b.y)/(a.norm()*b.norm());
}

inline f_vec add(const f_vec& a, const f_vec& b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

inline f_vec substract(const f_vec& a, const f_vec& b)
{
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

class GaussianDistr
{
    public:

    GaussianDistr(double mean=0.0, double std=1.0, double min = 0, double max = 0):
        _generator(std::random_device{}()), _distribution(mean, std), _max(max), _min(min)
    {}

    double draw() { 
        double val = _distribution(_generator); 
        if(abs(_min-_max)<10e-4) return val;
        else if(val < _min) return _min;
        else if(val > _max) return _max;
        else return val;
        }
    void reset(std::normal_distribution<> newdist) { _distribution = newdist; }

    private:
    std::minstd_rand _generator;
    std::normal_distribution<> _distribution;

    double _min;
    double _max;
};

