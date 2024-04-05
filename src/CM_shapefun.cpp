#include"CM_shapefun.hpp"


/* Basic shape functions */
namespace bsf
{
    cm_int radius_bound(double h, double r, const Grain& grain)
    {
        if(r <= grain.ref_column_rad) return 1;
        else return Grain::NON_VALID;
    }

    cm_int top_test(double h, double r, const Grain& grain)
    {
        if(r <= (1.0 - (h - (1.0 - grain.h0_norm_top_region)*grain.ref_length)/(grain.h0_norm_top_region*grain.ref_length))*grain.ref_column_rad
            && r <= grain.ref_column_rad) return 1;
        else return Grain::NON_VALID;
    }
}

cm_int t_smth(double h, double r, const Grain& grain)
{
    if(r <= grain.ref_column_rad) return 1;
    else return Grain::NON_VALID;
}

cm_int t_fth(double h, double r, const Grain& grain)
{
    if(r <= grain.ref_column_rad) return 1;
    else return Grain::NON_VALID;
}

cm_int t_tp(double h, double r, const Grain& grain)
{
    if(r <= grain.ref_column_rad) return 1;
    else return Grain::NON_VALID;
}