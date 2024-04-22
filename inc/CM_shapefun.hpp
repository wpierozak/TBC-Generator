#pragma once
#include"CM_mstypes.hpp"

typedef SHAPE_FUNCTION((*sfptr));

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
    }

    namespace top
    {
        SHAPE_FUNCTION(parabolic_profile);
        SHAPE_FUNCTION(parabolic_profile_w);
    }
}
