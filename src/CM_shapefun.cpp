#include"CM_shapefun.hpp"


/* Basic shape functions */
namespace bsf
{
    namespace smooth
    {
        SHAPE_FUNCTION(radius_bound)
        {
            if(r <= grain.ref_column_rad) return 1;
            else return Grain::NON_VALID;
        }

        SHAPE_FUNCTION(radius_bound_w)
        {
            if(r <= grain.ref_column_rad || (r-grain.ref_column_rad)/h < tan(grain.angle_of_widen))
            return 1;
            else return Grain::NON_VALID;
        }
    }

    namespace feathered
    {
        SHAPE_FUNCTION(feather)
        {
            if(r < grain.ref_column_rad) return 1;
            if( grain.f_param[2]*(h - grain.f_param[0]) + grain.ref_column_rad >= r && 
                r >= grain.f_param[3]*(h - grain.f_param[0] - grain.f_param[1]) + grain.ref_column_rad)
            {
                return 1;
            }
            return Grain::NON_VALID;
        }

        SHAPE_FUNCTION(feather_w)
        {
            if(r < grain.ref_column_rad || (r-grain.ref_column_rad)/h < tan(grain.angle_of_widen)) return 1;
            if( grain.f_param[2]*(h - grain.f_param[0]) + grain.ref_column_rad >= r && 
                r >= grain.f_param[3]*(h - grain.f_param[0] - grain.f_param[1]) + grain.ref_column_rad)
            {
                return 1;
            }
            return Grain::NON_VALID;
        }

        SHAPE_FUNCTION(multi_feather_w)
        {
            if(r < grain.ref_column_rad || (r-grain.ref_column_rad)/h < tan(grain.angle_of_widen)) return 1;
            for(double i = 1.0; i <= grain.f_param[4]; i+=1.0)
            {
                if( grain.f_param[2]*(h - grain.f_param[0] - grain.f_param[1]*(i-1.0)) + grain.ref_column_rad >= r && 
                    r >= grain.f_param[3]*(h - grain.f_param[0] - grain.f_param[1]*i) + grain.ref_column_rad)
                {
                    return 1;
                }
            }
            return Grain::NON_VALID;
        }
    }

    namespace top
    {
       SHAPE_FUNCTION(parabolic_profile)
        {
            double rn = r/grain.ref_column_rad;
            double hn = (h - grain.ref_length*(1.0 - grain.h0_norm_top_region))/(grain.h0_norm_top_region*grain.ref_length);
            double hp = (grain.t_param[1]+ grain.t_param[0]*rn)*rn + grain.t_param[2];
            if( hn < hp && r < grain.ref_column_rad) return 1;
            else return Grain::NON_VALID;
        }
        SHAPE_FUNCTION(parabolic_profile_w)
        {
            double rn = r/(grain.ref_column_rad + grain.ref_length*(1.0 - grain.h0_norm_top_region) * tan(grain.angle_of_widen));
            double hn = (h - grain.ref_length*(1.0 - grain.h0_norm_top_region))/(grain.h0_norm_top_region*grain.ref_length);
            double hp = (grain.t_param[1]+ grain.t_param[0]*rn)*rn + grain.t_param[2];
            if( hn < hp) return 1;
            else return Grain::NON_VALID;
        }
    }
}
