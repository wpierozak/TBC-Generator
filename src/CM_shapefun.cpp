#include"CM_shapefun.hpp"


/* Basic shape functions */
namespace bsf
{
    namespace smooth
    {
        cm_int radius_bound(double h, double r, const Grain& grain)
        {
            if(r <= grain.ref_column_rad) return 1;
            else return Grain::NON_VALID;
        }

        cm_int radius_bound_w(double h, double r, const Grain& grain)
        {
            if(r <= grain.ref_column_rad || (r-grain.ref_column_rad)/h < tan(grain.angle_of_widen))
            return 1;
            else return Grain::NON_VALID;
        }
    }

    cm_int top_test(double h, double r, const Grain& grain)
    {
        if(r <= (1.0 - (h - (1.0 - grain.h0_norm_top_region)*grain.ref_length)/(grain.h0_norm_top_region*grain.ref_length))*grain.ref_column_rad
            && r <= grain.ref_column_rad) return 1;
        else return Grain::NON_VALID;
    }

    cm_int parabolic_top_test(double h, double r, const Grain& grain)
    {
        const double a = -0.25;
        const double b = 0;
        const double c = 0.25;
        double rn = r/(grain.ref_column_rad + grain.ref_length*(grain.h0_norm_smooth_region + grain.h0_norm_feathered_region) * tan(grain.angle_of_widen));
        double hn = (h - grain.ref_length*(grain.h0_norm_smooth_region + grain.h0_norm_feathered_region))/(grain.h0_norm_top_region*grain.ref_length);
        double hp = (b + a*rn)*rn + c;
        if( hn < hp ) return 1;
        else return Grain::NON_VALID;
    }

    namespace top
    {
        cm_int parabolic_profile(double h, double r, const Grain& grain)
        {
            double rn = r/grain.ref_column_rad;
            double hn = (h - grain.ref_length*(1.0 - grain.h0_norm_top_region))/(grain.h0_norm_top_region*grain.ref_length);
            double hp = (grain.top_section_function_param [1]+ grain.top_section_function_param[0]*rn)*rn + grain.top_section_function_param[2];
            if( hn < hp && r < grain.ref_column_rad) return 1;
            else return Grain::NON_VALID;
        }
        cm_int parabolic_profile_w(double h, double r, const Grain& grain)
        {
            double rn = r/(grain.ref_column_rad + grain.ref_length*(grain.h0_norm_smooth_region + grain.h0_norm_feathered_region) * tan(grain.angle_of_widen));
            double hn = (h - grain.ref_length*(1.0 - grain.h0_norm_top_region))/(grain.h0_norm_top_region*grain.ref_length);
            double hp = (grain.top_section_function_param [1]+ grain.top_section_function_param[0]*rn)*rn + grain.top_section_function_param[2];
            if( hn < hp) return 1;
            else return Grain::NON_VALID;
        }
    }
}
