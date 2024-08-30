#pragma once
#include<vector>
#include<memory>
#include"CM_basictypes.hpp"

enum class Resolution {LOW=0, HIGH=1};

struct Grain
{
    /* Grain global ID */
    uint16_t ID;

    /* Coordinates of the center cell of the base of a column*/
    f_vec center;

    /* Growth tensor' coordinates */
    f_vec orientation;

    Resolution resolution;
};

typedef std::vector<Grain> grains_array;

/* Contains information needed to define grain properties */
struct Layer
{
    Layer() = default;
    Layer(const Layer& obj)
    {
        tilt_stddev = obj.tilt_stddev;
        prefered_orientation = obj.prefered_orientation;
        grainsNumber = obj.grainsNumber;
        layer_height = obj.layer_height;
    }

    double tilt_stddev;

    f_vec prefered_orientation;
    _int grainsNumber;
    _long_int layer_height;
};

