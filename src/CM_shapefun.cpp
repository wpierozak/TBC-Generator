#include"CM_shapefun.hpp"

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