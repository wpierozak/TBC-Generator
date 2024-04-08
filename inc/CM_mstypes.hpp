#pragma once
#include<vector>
#include"CM_basictypes.hpp"

struct Grain
{
    /* Methods */
    void print() const;

    /* Grain global ID */
    cm_size ID;

    /* Coordinates of the center cell of the base of a column*/
    f_vec center;

    /* Growth tensor' coordinates */
    f_vec growth_tensor;

    /* Reference bound for the angle between growth vector and relative position vector, stored as its cos value */
    double angle_of_widen;

    /* Reference bound for the RPV norm */
    double ref_length;

    /* Reference bound for h0 norm of smooth region */
    double h0_norm_smooth_region;

    /* Reference bound for h0 norm of feathered region */
    double h0_norm_feathered_region;

    /* Reference bound for h0 norm of top region */
    double h0_norm_top_region;

    static const cm_int NON_VALID = __INT64_MAX__;

    cm_int (*smooth_section_function)(double, double, const Grain&);
    #define SMOOTH_FUNCTION_OREDER 1
    double smooth_section_function_coeff[SMOOTH_FUNCTION_OREDER + 1];

    /* 
        Definition of the column shape within feathered region
        Shape is defined by two cubic functions:
            -> gap function - determines the width of gap between main column and a feather and an inner edge of a feather 
            -> feather function - determines the shape and width of the outer edge of a feather 
    */
    cm_int (*feathered_section_function)(double, double, const Grain&);
    #define FEATHERED_FUNCTION_OREDER 3
    double feathered_section_function_coeff[FEATHERED_FUNCTION_OREDER + 1];

    /* 
        Definition of the column shape within top region
        Shape is defined by splain of two linear function. Definition of the shape demands three parameters:
            -> gradient of the "left" function
            -> connecting point coordinate
            -> gradient of the "right" function 
    */
    cm_int (*top_section_function)(double, double, const Grain&);
    #define TOP_REGION_PARAM_NUM 3
    double top_section_function_param[TOP_REGION_PARAM_NUM];
    
    /* Reference column radius */
    double ref_column_rad;

    /* Maximum column radius */
    double max_column_rad;
};

typedef std::vector<Grain> grains_array;

void copy(Grain& dest, const Grain& src);

/* Contains information needed to define grain properties */
struct Microstructure_Properties
{
    /* Methods */
    void print() const;

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

