#include<random>
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

        SHAPE_FUNCTION(rand_feather)
        {
            if(r < grain.ref_column_rad || (r-grain.ref_column_rad)/h < tan(grain.angle_of_widen)) return 1;
            for(int i = 0; i < grain.f_param.back(); i++)
            {
                if( grain.f_param[i*4]*(h - grain.f_param[i*4+3]) + grain.ref_column_rad >= r && 
                    r >= grain.f_param[i*4]*grain.f_param[i*4+1]*(h - grain.f_param[i*4+3] - grain.f_param[i*4+2]) + grain.ref_column_rad)
                {
                    return 1;
                }
            }
            return Grain::NON_VALID;
        }

        SETUP_SHAPE_FUNTION(rand_feather_s)
        {
            std::uniform_real_distribution<double> uniform(0.1,1);
            std::uniform_real_distribution<double> half_uniform(0.3,1);
            std::minstd_rand rr(std::random_device{}());
            double alpha_max = coef[0];
            double p_max = coef[1];
            double dh_max = coef[2];
            double h1_min = coef[3];
            double h1_max = coef[4];
            int itr = coef[5];

            itr = floor(uniform(rr)*itr);
            itr = (itr >= 1) ? itr : 1;

            coef.resize(itr*4+1);
            coef.back() = itr;

            for(cm_int i = 0; i < coef.back(); i++)
            {
                coef[i*4] = tan(uniform(rr) * alpha_max);

                coef[i*4+1] = uniform(rr) * p_max;
                if(coef[i*4+1] != 0) coef[i*4+1] = (1.0+coef[i*4+1])/coef[i*4+1];

                coef[i*4+2] = uniform(rr) * dh_max;

                coef[i*4+3] = uniform(rr) * (h1_max-h1_min) + h1_min;
            }
        }

        SHAPE_FUNCTION(gaussian_feather)
        {
            
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
