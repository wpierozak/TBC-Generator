#pragma once
#include<iostream>
#include<cmath>
#include <cstdint>
#include<vector>
#include<string>
#include<memory>

/* State of an empty cell */
#define EMPTY 0

typedef int32_t cm_pos;
typedef int64_t cm_int;
typedef uint16_t cm_state;
typedef uint8_t cm_colorampl;
typedef uint8_t cm_smallsize;
typedef size_t cm_size;

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

enum class BC{absorption, bouncy, periodic};
enum class MsFileFormat{xyz, xyzrgb};

struct Grain
{
    /* Grain global ID */
    cm_size ID;

    /* Coordinates of the center cell of the base of a column*/
    f_vec center;

    /* Growth tensor' coordinates */
    f_vec growth_tensor;

    /* Reference bound for the angle between growth vector and relative position vector, stored as its cos value */
    double cos_phi_ub;

    /* Reference bound for the RPV norm */
    double ref_length;

    /* Reference bound for h0 norm of smooth region */
    double h0_norm_smooth_region;

    /* Reference bound for h0 norm of feathered region */
    double h0_norm_feathered_region;

    /* Reference bound for h0 norm of top region */
    double h0_norm_top_region;

    static const cm_int NON_VALID = __INT64_MAX__;

    cm_int (*smooth_region_function)(double, double, const Grain&);
    #define SMOOTH_FUNCTION_OREDER 1
    double smooth_region_function_coeff[SMOOTH_FUNCTION_OREDER + 1];

    /* 
        Definition of the column shape within feathered region
        Shape is defined by two cubic functions:
            -> gap function - determines the width of gap between main column and a feather and an inner edge of a feather 
            -> feather function - determines the shape and width of the outer edge of a feather 
    */
    cm_int (*feathered_region_function)(double, double, const Grain&);
    #define FEATHERED_FUNCTION_OREDER 3
    double feathered_region_function_coeff[FEATHERED_FUNCTION_OREDER + 1];

    /* 
        Definition of the column shape within top region
        Shape is defined by splain of two linear function. Definition of the shape demands three parameters:
            -> gradient of the "left" function
            -> connecting point coordinate
            -> gradient of the "right" function 
    */
    cm_int (*top_region_function)(double, double, const Grain&);
    #define TOP_REGION_PARAM_NUM 3
    double top_region_function_param[TOP_REGION_PARAM_NUM];
    
    /* Reference column radius */
    double ref_column_rad;

    /* Maximum column radius */
    double max_column_rad;
};

void printGrain(const Grain& grain);

typedef std::vector<Grain> grains_array;

void copy(Grain& dest, const Grain& src);

/* Contains information needed to define grain properties */
struct Microstructure_Properties
{
    /* Maximum value of angle widen of a column */
    double max_angle_of_widen;

    /* Maximum tilt of a column in the YZ and XY plane */
    double max_tilt;

    /* Minimum tilt of a column in the YZ and XY plane */
    double min_tilt;

    /* Length of the smooth region in % of total length*/
    double smooth_region_length;
    
    /* Maximum absolute variance of the smooth region in % of total length */
    double smooth_region_length_var;

    /* Length of the feathered region in % of total length*/
    double feathered_region_length;

    /* Maximum absolute variance of the feathered region in % of total length */
    double feathered_region_length_var;

    /* Length of the top region in % of total length*/
    double top_region_length;

    /* Maximum absolute variance of the top region in % of total length */
    double top_region_length_var;

    /* Minimum length of a column */
    double min_length;

    /* Maximum length of a column */
    double max_length;

    /* Maximum value for the reference radius */
    double max_reference_radius;
};



void printMicrostructureProperties(const Microstructure_Properties& properties);


struct Neighbourhood
{
    Neighbourhood()= default;
    Neighbourhood(const Neighbourhood& n):
        dx0(n.dx0), dx1(n.dx1), dy0(n.dy0), dy1(n.dy1), dz0(n.dz0), dz1(n.dz1) {}
    Neighbourhood(cm_pos x0, cm_pos x1, cm_pos y0, cm_pos y1, cm_pos z0, cm_pos z1):
        dx0(x0), dx1(x1), dy0(y0), dy1(y1), dz0(z0), dz1(z1) {}
    cm_pos dx0, dx1;
    cm_pos dy0, dy1;
    cm_pos dz0, dz1;
};  

class Domain
{
    private:
    std::shared_ptr<cm_state[]> buffer;

    public:
    Domain() = delete;
    Domain(const Domain&) = default;
    Domain& operator=(const Domain&) = delete;
    Domain(cm_pos dimX, cm_pos dimY, cm_pos dimZ, Neighbourhood neighbourhood);

    static const cm_state VOID{static_cast<cm_state>(-1)};
    const cm_pos dimX;
    const cm_pos dimY;
    const cm_pos dimZ;
    const Neighbourhood neighbourhood;

    BC bc;

    cm_state& operator()(cm_pos x, cm_pos y, cm_pos z)
    {
        return buffer[(y*dimZ + z)*dimX + x];
    }

    cm_state state(cm_pos x, cm_pos y, cm_pos z) const; 
};

struct Configuration
{
    std::unique_ptr<Domain> domain;

    grains_array grains;
    cm_size grainsNumber;

    Microstructure_Properties msp;

    cm_smallsize threadsNum;

    std::string outputFile;
    std::string inputFile;
    std::string outputDir;
    MsFileFormat msFileFormat;
};