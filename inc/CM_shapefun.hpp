#pragma once
#include"CM_config.hpp"

typedef cm_int (*sfptr)(double, double, const Grain&);

namespace bsf
{
    namespace smooth
    {
        cm_int radius_bound(double h, double r, const Grain& grain);
        cm_int radius_bound_w(double h, double r, const Grain& grain);
    }
    cm_int top_test(double h, double r, const Grain& grain);
    cm_int parabolic_top_test(double h, double r, const Grain& grain);
    namespace top
    {
        cm_int parabolic_profile(double h, double r, const Grain& grain);
        cm_int parabolic_profile_w(double h, double r, const Grain& grain);
    }
}
