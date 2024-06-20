#include<iostream>
#include<omp.h>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<random>
#include<unordered_map>
#include"CM_generation.hpp"

double noise()
{
    static std::minstd_rand generator(std::random_device{}());
    static std::uniform_real_distribution<> distribution(-1,1);
    return distribution(generator);
}

double singleFieldCalculation(f_vec pos, const Grain& grain)
{
    const f_vec& growth_tensor = grain.growth_tensor;

    f_vec rpv = { pos.x - grain.center.x,  pos.y - grain.center.y, pos.z - grain.center.z};

    double h = dotProduct(rpv, growth_tensor); 
    double r = crossProduct(rpv, growth_tensor).norm();
    f_vec hg = growth_tensor*h;
    double phi = acos(dotProduct(substract(pos, hg), grain.r_vector)/((r!=0)?r:1));

    if( h <= grain.ref_length*(1.0-grain.top_fraction))
    {
        switch(grain.resolution)
        {
            case Resolution::LOW:
            //if(r <= grain.ref_column_rad || (r-grain.ref_column_rad)/h < tan(grain.angle_of_widen))
            return (r - tan(grain.angle_of_widen)*h - grain.ref_column_rad )* pow(grain.growth_tensor.y/grain.growth_tensor.norm(),2);
            //else return Grain::NON_VALID;
            case Resolution::HIGH:
            break;
        }
    }
    else 
    {
        double rn = r/(grain.ref_column_rad + grain.ref_length*(1.0 - grain.top_fraction) * tan(grain.angle_of_widen));
        double hn = (h - grain.ref_length*(1.0 - grain.top_fraction))/(grain.top_fraction*grain.ref_length);
        double hp = grain.top_parabola_coeff*(rn*rn - 1.0);
        if( hn < hp) return r - tan(grain.angle_of_widen)*h - grain.ref_column_rad;
        else return Grain::NON_VALID;
    }

    return Grain::NON_VALID;
}

void runTask(Task* task)
{
    std::minstd_rand generator(std::random_device{}());
    std::uniform_int_distribution<> dist(0,100);

    std::list<cm_state> grains_neighbourhood;
    for(cm_pos y = task->input.y0; y < task->input.y1; y++)
    for(cm_pos z = task->input.z0; z < task->input.z1; z++)
    for(cm_pos x = task->input.x0; x < task->input.x1; x++)
    {
        f_vec pos = {static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)};
        double bestfit = Grain::NON_VALID;
        scanNeighbourhood(pos, *task->input.domain, grains_neighbourhood);

        for(cm_state grain_idx: grains_neighbourhood)
        {
            Grain& grain = task->input.grains[grain_idx];
            double fit = singleFieldCalculation(pos, grain);
            if(fit >= bestfit) continue;
            (*task->input.domain)(x,y,z) = grain_idx;
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

void Generator::run()
{
    std::list<cm_state> grains_neighbourhood;
    for(cm_pos y = m_subspace.y0; y < m_subspace.y1; y++)
    for(cm_pos z = m_subspace.z0; z < m_subspace.z1; z++)
    for(cm_pos x = m_subspace.x0; x < m_subspace.x1; x++)
    {
        f_vec pos = {static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)};
        double bestfit = Grain::NON_VALID;
        scanNeighbourhood(pos, m_domain, grains_neighbourhood);

        for(cm_state grain_idx: grains_neighbourhood)
        {
            Grain& grain = m_grains[grain_idx];
            double fit = singleFieldCalculation(pos, grain);
            if(fit >= bestfit) continue;
            m_domain(x,y,z) = grain_idx;
            bestfit = fit;
        }
    }
}

void Generator::fitness_value(f_vec pos, const Grain& grain)
{
}