#include<iostream>
#include<cmath>
#include<algorithm>
#include<memory>
#include<ctime>
#include<random>
#include"CM_setup.hpp"
#include"CM_logs.hpp"

/* Global variables for random number generation in the grain set up process */
std::minstd_rand gl_rand_gen(std::random_device{}());
std::uniform_real_distribution<double> lu_dist(-1, 1);
std::uniform_real_distribution<double> p_dist(0, 1);
std::normal_distribution<double> n_dist(0.7,0.2);

void Nucleator::nucleate(Domain& domain, cm_pos y0, cm_int g0, Layer& msp)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(0, 1);

    uint16_t grain_ID = g0;
    
    cm_pos dimX = domain.dimX;
    cm_pos dimZ = domain.dimZ;
    cm_pos dX = (dimX + sqrt(msp.grainsNumber) - 1)/ceil(sqrt(msp.grainsNumber));
    cm_pos dZ = (dimZ + sqrt(msp.grainsNumber) - 1)/floor(sqrt(msp.grainsNumber));

    for(cm_pos z = 0; z < dimZ; z+=dZ)
        for(cm_pos x = 0; x < dimX; x+=dX)
        {
            /* center */
            m_grains.insert({grain_ID, Grain()});
            m_grains[grain_ID].center = {round( x), double(y0), round( z)};
            if(m_grains[grain_ID].center.x >= dimX) m_grains[grain_ID].center.x = dimX - 1.0;
            if(m_grains[grain_ID].center.z >= dimZ) m_grains[grain_ID].center.z = dimZ - 1.0;
            /* ID */
            m_grains[grain_ID].ID = grain_ID;
            /* Growth tensor */
            setGrowthTensor(m_grains[grain_ID], msp);

            domain(m_grains[grain_ID].center.x, y0, m_grains[grain_ID].center.z) = {m_grains[grain_ID].ID,0};
            
            grain_ID++;
            #ifdef DEBUG
            if(LogManager::Manager().logmode()) LogManager::Manager().printGrainData(m_grains[grain_ID-1]);
            #endif 
            if(grain_ID == msp.grainsNumber) break;
        }
}



void Nucleator::setGrowthTensor(Grain& grain, Layer& msp)
{
    GaussianDistr tilt(msp.tilt.mean, msp.tilt.stddev, msp.tilt.min, msp.tilt.max);
    double xy_angle = radians(tilt.draw());
    double zy_angle = radians(tilt.draw());

    grain.growth_tensor.x = sin(xy_angle);
    grain.growth_tensor.z = sin(zy_angle);
    grain.growth_tensor.y = 1.0;

    normalize(grain.growth_tensor);
}
