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
    double time;

    cell(uint16_t s = -1, double t = 0): state(s), time(t)
    {

    }

    operator int() { return state; }
    // Implement equality comparison operator (necessary for unordered containers)
    bool operator==(const cell& other) const
    {
        return state == other.state && time == other.time;
    }
};


constexpr double PI_180 = M_PI/180.0;

inline double radians(double degrees) { return degrees * PI_180; }

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

    double operator*(const f_vec& a) const
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

    double cos(const f_vec& b) const
    {
        return ( (*this)*b  )/ (this->norm() * b.norm());
    }

    void normalize()
    {
        double norm = this->norm();
        x = x/norm;
        y = y/norm;
        z = z/norm;
    }
};


class GaussianDistr
{
    public:

    GaussianDistr(double mean=0.0, double std=1.0, double min = 0, double max = 0):
        _generator(std::random_device{}()), _distribution(mean, std), _max(max), _min(min)
    {}

    GaussianDistr(const GaussianDistr& obj): 
        _generator(std::random_device{}()), _distribution(obj.mean(), obj.std()), _max(obj.max()), _min(obj.min())
    {}

    double draw() { 
        double val = _distribution(_generator); 
        if(abs(_min-_max)<10e-4) return val;
        else if(val < _min) return _min;
        else if(val > _max) return _max;
        else return val;
        }
    void reset(std::normal_distribution<> newdist) { _distribution = newdist; }

    double mean() const {return _distribution.mean();}
    double std() const {return _distribution.stddev();}
    double min() const {return _min;}
    double max() const {return _max;}

    private:
    std::minstd_rand _generator;
    std::normal_distribution<> _distribution;

    double _min;
    double _max;
};

std::ostream& operator<<(std::ostream& os, const GaussianDistr gd);
