
#include<iostream>
#include<cmath>
#include<algorithm>
#include<memory>
#include<ctime>
#include<random>
#include"CM_setup.hpp"

/* Global variables for random number generation in the grain set up process */
std::minstd_rand gl_rand_gen(std::random_device{}());
std::uniform_real_distribution<double> lu_dist(-1, 1);
std::uniform_real_distribution<double> p_dist(0, 1);
std::normal_distribution<double> n_dist(0.5,0.25);


void defineTasks(const Configuration& config, tasks_array& tasks)
{
    int threadsNumber = config.threadsNum;
    tasks.resize(threadsNumber);

    cm_pos dx = (config.domain->dimX + threadsNumber - 1)/ceil(sqrt(threadsNumber));
    cm_pos dz = (config.domain->dimZ + threadsNumber - 1)/floor(sqrt(threadsNumber));
    cm_pos y0 = 1;
    cm_pos y1 = 2;

    cm_int counter = 0;

    for(cm_pos z = 0; z < config.domain->dimZ; z+=dz)
    for(cm_pos x = 0; x < config.domain->dimX; x+=dx)
    {
        tasks[counter].input.domain = std::make_unique<Domain>(*config.domain);

        tasks[counter].input.x0 = x;
        tasks[counter].input.x1 = ((x + dx) < config.domain->dimX)? x + dx: config.domain->dimX;
        tasks[counter].input.y0 = y0;
        tasks[counter].input.y1 = y1;
        tasks[counter].input.z0 = z;
        tasks[counter].input.z1 = ((z + dz) < config.domain->dimZ)? z + dz: config.domain->dimZ; 


        std::copy(config.grains.begin(), config.grains.end(), std::back_insert_iterator(tasks[counter].input.grains));   
        counter++;              
    }
}

std::pair<cm_pos, cm_pos> findDiv(cm_pos dimX, cm_pos dimZ)
{
    std::pair<cm_size, cm_size> res;
    cm_pos field = dimX*dimZ;
    cm_pos min_diff = field+1;
    for(cm_pos n = 1; n < field; n++)
    {
        if(field % n != 0) continue;
        cm_pos r = field/n;
        if(abs(r-n) < min_diff)
        {
            res.first = r;
            res.second = n;
            min_diff = abs(r-n);
        }
    }
    return res;
}

void nucleation(Configuration& config)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(0, 1);
    
    cm_pos dimX = config.domain->dimX;
    cm_pos dimZ = config.domain->dimZ;

    grains_array grains;
    grains.resize(config.grainsNumber);
    cm_size n = 0;

    auto div = findDiv(dimX, dimZ);
    double dX = static_cast<double>(dimX/div.first);
    double dZ = static_cast<double>(dimZ/div.second);

    for(double z = 0; z < dimZ; z+=dZ)
        for(double x = 0; x < dimX; x+=dX)
        {
            /* center */
            grains[n].center = {round(dist(gen)*dX + x), 0.0, round(dist(gen)*dZ + z)};
            if(grains[n].center.x >= dimX) grains[n].center.x = dimX - 1.0;
            if(grains[n].center.z >= dimZ) grains[n].center.z = dimZ - 1.0;
            /* ID */
            grains[n].ID = n;
            /* Growth tensor */
            setGrowthTensor(grains[n], config.msp);
            /* cos_phi_ub */
            setMaxWidenAngle(grains[n], config.msp);
            /* h0_norm_smooth_region */
            setSmoothRegionLen(grains[n], config.msp);
            /* h0_norm_feathered_region */
            setFeatheredRegionLen(grains[n], config.msp);
            /* h0_norm_top_region */
            setTopRegionLen(grains[n], config.msp);
            /* ref_column_rad */
            setReferenceRadius(grains[n], config.msp);
        }

    std::copy(grains.begin(), grains.end(), std::back_insert_iterator(config.grains));
}

/* Defines grow tensor with regarding in-code parameters */
void setGrowthTensor(Grain& grain, const Microstructure_Properties& msp)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<> rand_angle(msp.min_tilt * M_PI / 180.0, msp.max_tilt * M_PI / 180.0);

    double xy_angle = rand_angle(gen);
    double zy_angle = rand_angle(gen);

    grain.growth_tensor.x = sin(xy_angle);
    grain.growth_tensor.z = sin(zy_angle);
    grain.growth_tensor.y = 1.0;

    normalize(grain.growth_tensor);
}

/* Defines bound for column width by bounding the angle between growth tensor and relative position vector*/
void setMaxWidenAngle(Grain& grain, const Microstructure_Properties& msp)
{
    grain.cos_phi_ub = cos((180.0 - msp.max_angle_of_widen * 0.5) * M_PI / 180.0);
}

void setSmoothRegionLen(Grain& grain, const Microstructure_Properties& msp)
{
    grain.h0_norm_smooth_region = msp.smooth_region_length + lu_dist(gl_rand_gen)*msp.smooth_region_length_var;
}

void setFeatheredRegionLen(Grain& grain, const Microstructure_Properties& msp)
{
    grain.h0_norm_feathered_region = msp.feathered_region_length + lu_dist(gl_rand_gen)*msp.feathered_region_length_var;
}

void setTopRegionLen(Grain& grain, const Microstructure_Properties& msp)
{
    grain.h0_norm_top_region = msp.top_region_length + lu_dist(gl_rand_gen)*msp.top_region_length_var;
}

void setReferenceRadius(Grain& grain, const Microstructure_Properties& msp)
{
    grain.ref_column_rad = msp.max_reference_radius*n_dist(gl_rand_gen);
}
