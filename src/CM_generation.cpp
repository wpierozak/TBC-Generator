#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<random>
#include"CM_generation.hpp"

cm_int singleFieldCalculation(f_vec pos, const Grain& grain)
{
    const f_vec& growth_tensor = grain.growth_tensor;

    f_vec rpv = { pos.x - grain.center.x,  pos.y - grain.center.y, pos.z - grain.center.z};

    double h = (growth_tensor.x * rpv.x + growth_tensor.y * rpv.y + growth_tensor.z * rpv.z);
    double r = crossProduct(rpv, growth_tensor).norm();

    if( h <= grain.h0_norm_smooth_region)
    {
        return grain.smooth_region_function(h,r, grain.smooth_region_function_coeff);
    }
    else if( h <= grain.h0_norm_feathered_region)
    {
        return grain.feathered_region_function(h, r, grain.feathered_region_function_coeff);
    }
    else if( h <= grain.h0_norm_top_region)
    {
        return grain.top_region_function(h, r, grain.top_region_function_param);
    }
}

void runTask(Task& task)
{
    std::list<cm_state> grains_neighbourhood;
    for(cm_pos y = task.input.y0; y < task.input.y1; y++)
    for(cm_pos z = task.input.z0; z < task.input.z1; z++)
    for(cm_pos x = task.input.x0; x < task.input.x1; x++)
    {
        f_vec pos = {x,y,z};
        cm_int bestfit = __INT64_MAX__;
        scanNeighbourhood(pos, *task.input.domain, grains_neighbourhood);
        for(cm_state grain_idx: grains_neighbourhood)
        {
            Grain& grain = task.input.grains[grain_idx];
            cm_int fit = singleFieldCalculation(pos, grain);
            if(fit >= bestfit) continue;
            (*task.input.domain)(x,y,z) = grain_idx;
            bestfit = fit;
        }
    }
}


void scanNeighbourhood(f_vec pos, const Domain& domain, std::list<cm_state>& grains)
{
    grains.clear();

    for(cm_pos dy = domain.neighbourhood.dy0; dy <= domain.neighbourhood.dy1; dy++)
    for(cm_pos dz = domain.neighbourhood.dz0; dz <= domain.neighbourhood.dz1; dz++)
    for(cm_pos dx = domain.neighbourhood.dx0; dx <= domain.neighbourhood.dx1; dx++)
    {
        cm_state n = domain.state(pos.x + dx, pos.y + dy, pos.z + dz);
        if(n == Domain::VOID) continue;
        if(std::find(grains.begin(), grains.end(), n) != grains.end()) continue;
        grains.push_back(n);
    }
}