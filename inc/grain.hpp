#pragma once
#include"basictypes.hpp"

struct Grain
{
    /* Grain global ID */
    uint16_t ID;

    /* Coordinates of the center cell of the base of a column*/
    f_vec center;

    /* Growth tensor' coordinates */
    f_vec orientation;

    float theta[27];
};
