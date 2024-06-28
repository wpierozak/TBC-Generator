#pragma once
#include<vector>
#include<memory>
#include"CM_basictypes.hpp"

enum class Resolution {LOW=0, HIGH=1};

struct Grain
{
    /* Grain global ID */
    cm_state ID;

    /* Coordinates of the center cell of the base of a column*/
    f_vec center;

    /* Growth tensor' coordinates */
    f_vec growth_tensor;

    f_vec r_vector;

    /* Reference bound for the angle between growth vector and relative position vector, stored as its cos value */
    double angle_of_widen;

    /* Reference bound for the RPV norm */
    double ref_length;

    double top_fraction;
    double top_parabola_coeff;
    Resolution resolution;

    static const cm_int NON_VALID = 0;
    
    /* Reference column radius */
    double ref_column_rad;

    /* Generation coefficients */
    double y_to_norm2;
    double tan_angle_of_widen;
};

typedef std::vector<Grain> grains_array;

/* Contains information needed to define grain properties */
struct Microstructure_Properties
{
    Microstructure_Properties() = default;
    Microstructure_Properties(const Microstructure_Properties& obj)
    {
        length = std::make_shared<GaussianDistr>(*obj.length);
        radius = std::make_shared<GaussianDistr>(*obj.radius);
        tilt = std::make_shared<GaussianDistr>(*obj.tilt);
        widen = std::make_shared<GaussianDistr>(*obj.widen);
        top_frac = std::make_shared<GaussianDistr>(*obj.top_frac);
        resolution = obj.resolution;
        grainsNumber = obj.grainsNumber;
        layer_height = obj.layer_height;
    }

    std::shared_ptr<GaussianDistr> length;
    std::shared_ptr<GaussianDistr> radius;
    std::shared_ptr<GaussianDistr> tilt;
    std::shared_ptr<GaussianDistr> widen;
    std::shared_ptr<GaussianDistr> top_frac;

    cm_int grainsNumber;
    cm_pos layer_height;

    Resolution resolution;

    void print(std::ostream& os) const;
};
