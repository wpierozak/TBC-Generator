#pragma once
#include"CM_datatypes.hpp"

namespace bsf
{
    cm_int radius_bound(double h, double r, const Grain& grain);
    cm_int top_test(double h, double r, const Grain& grain);
}

cm_int t_smth(double h, double r, const Grain& grain);

cm_int t_fth(double h, double r, const Grain& grain);

cm_int t_tp(double h, double r, const Grain& grain);