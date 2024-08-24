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
    f_vec growth_tensor;

    Resolution resolution;
};

typedef std::vector<Grain> grains_array;

/* Contains information needed to define grain properties */
struct Layer
{
    Layer() = default;
    Layer(const Layer& obj)
    {
        tilt = obj.tilt;

        resolution = obj.resolution;
        grainsNumber = obj.grainsNumber;
        layer_height = obj.layer_height;
    }

    struct GaussianParam {double mean; double stddev; double min; double max; } tilt;

    cm_int grainsNumber;
    cm_pos layer_height;

    Resolution resolution;
};

