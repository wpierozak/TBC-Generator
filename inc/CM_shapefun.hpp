#pragma once
#include"CM_mstypes.hpp"

typedef SHAPE_FUNCTION((*sfptr));

#define SETUP_SHAPE_FUNTION(name)\
void name(std::vector<double>& coef)

namespace bsf
{
    namespace smooth
    {
        SHAPE_FUNCTION(radius_bound);
        SHAPE_FUNCTION(radius_bound_w);
    }

    namespace feathered
    {
        SHAPE_FUNCTION(feather);
        SHAPE_FUNCTION(feather_w);
        SHAPE_FUNCTION(multi_feather_w);
        SHAPE_FUNCTION(rand_feather);
        SETUP_SHAPE_FUNTION(rand_feather_s);
        /*
            Feather is created at a given height with probablility p(h)
            Magnitude of a feather is specified as m * ref_length
            Alfa is in range (0,90)
            Beta is calculated based on the
        */
        SHAPE_FUNCTION(gaussian_feather);
    }

    namespace top
    {
        SHAPE_FUNCTION(parabolic_profile);
        SHAPE_FUNCTION(parabolic_profile_w);
    }
}
