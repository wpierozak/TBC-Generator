#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<random>
#include"CM_generation.hpp"
#include"CM_parameters.hpp"

void growGrain(const TaskData& subdomain, const Grain& grain)
{
    f_vec cy = grain.center;
    const f_vec& growth_tensor = grain.growth_tensor;

    while(cy.y < subdomain.dimY)
    {
        for(cm_pos z = -grain.max_column_rad/2; z < grain.max_column_rad/2; z++)
        for(cm_pos x = -grain.max_column_rad/2; x < grain.max_column_rad/2; x++)
        {
            f_vec rpv = { static_cast<double>(x) - grain.center.x,  cy.y - grain.center.y, static_cast<double>(z) - grain.center.z};

            double h = (growth_tensor.x * rpv.x + growth_tensor.y * rpv.y + growth_tensor.z * rpv.z);
            double r = crossProduct(rpv, growth_tensor).norm();

            if( h <= grain.h0_norm_smooth_region)
            {
                if(grain.smooth_region_function(h,r, grain.smooth_region_function_coeff))
                {

                }
            }
            else if( h <= grain.h0_norm_feathered_region)
            {
                if(grain.feathered_region_function(h, r, grain.feathered_region_function_coeff))
                {

                }
            }
            else if( h <= grain.h0_norm_top_region)
            {
                if(grain.top_region_function(h, r, grain.top_region_function_param))
                {

                }
            }
        }

        cy = add(cy, grain.growth_tensor * (1.0/grain.growth_tensor.y));
    }
}