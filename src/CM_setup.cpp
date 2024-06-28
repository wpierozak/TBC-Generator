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

void Nucleator::nucleate(Domain& domain, cm_pos y0, cm_int g0, Microstructure_Properties& msp)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(0, 1);

    cm_state grain_ID = g0;
    
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
            setRVector(m_grains[grain_ID]);
             /* ref_length */
            setReferenceLength(m_grains[grain_ID], msp);
            /* cos_phi_ub */
            setMaxWidenAngle(m_grains[grain_ID], msp);
            /* h0_norm_top_region */
            setTopRegionLen(m_grains[grain_ID], msp);
            /* ref_column_rad */
            setReferenceRadius(m_grains[grain_ID], msp);
           

            m_grains[grain_ID].resolution = msp.resolution;

            m_grains[grain_ID].tan_angle_of_widen = tan(m_grains[grain_ID].angle_of_widen);
            m_grains[grain_ID].y_to_norm2 = pow(m_grains[grain_ID].growth_tensor.y/m_grains[grain_ID].growth_tensor.norm(),2);
            
            grain_ID++;
            #ifdef DEBUG
            if(LogManager::Manager().logmode()) LogManager::Manager().printGrainData(m_grains[grain_ID-1]);
            #endif 
            if(grain_ID == msp.grainsNumber) break;
        }

    std::vector<int> idxs;
    for(int i = g0; i < msp.grainsNumber+g0; i++) idxs.push_back(i);
    std::minstd_rand minstd(std::random_device{}());
    std::shuffle(idxs.begin(), idxs.end(), minstd);
    for(int grain_ID: idxs)
    {
        for(cm_pos dz = -m_grains[grain_ID].ref_column_rad; m_grains[grain_ID].center.z + dz < dimZ && dz < m_grains[grain_ID].ref_column_rad; dz++)
        for(cm_pos dx = -m_grains[grain_ID].ref_column_rad; m_grains[grain_ID].center.x + dx < dimX && dx < m_grains[grain_ID].ref_column_rad; dx++)
            {
                if(dx*dx + dz*dz > pow(m_grains[grain_ID].ref_column_rad, 2)) continue;
                if(m_grains[grain_ID].center.x + dx < 0 || m_grains[grain_ID].center.x + dx > dimX ||
                    m_grains[grain_ID].center.z + dz < 0 || m_grains[grain_ID].center.z + dz > dimZ) continue; 
                if(domain(m_grains[grain_ID].center.x + dx, y0, m_grains[grain_ID].center.z + dz) == Domain::VOID)
                domain(m_grains[grain_ID].center.x + dx, y0, m_grains[grain_ID].center.z + dz) = m_grains[grain_ID].ID;
            }
    }
}

void Nucleator::setMaxWidenAngle(Grain& grain, Microstructure_Properties& msp)
{
    grain.angle_of_widen = radians(msp.widen->draw());
}


void Nucleator::setTopRegionLen(Grain& grain, Microstructure_Properties& msp)
{
    grain.top_fraction = msp.top_frac->draw();
    grain.top_parabola_coeff = -0.5;
}

void Nucleator::setReferenceRadius(Grain& grain, Microstructure_Properties& msp)
{
    grain.ref_column_rad = msp.radius->draw();
}

void Nucleator::setReferenceLength(Grain& grain, Microstructure_Properties& msp)
{
    grain.ref_length = msp.length->draw();
}

void Nucleator::setRVector(Grain& grain)
{
    f_vec i = {1,0,0};
    grain.r_vector = crossProduct(i, grain.growth_tensor);
    normalize(grain.r_vector);
}

void Nucleator::setGrowthTensor(Grain& grain, Microstructure_Properties& msp)
{
    double xy_angle = radians(msp.tilt->draw());
    double zy_angle = radians(msp.tilt->draw());

    grain.growth_tensor.x = sin(xy_angle);
    grain.growth_tensor.z = sin(zy_angle);
    grain.growth_tensor.y = 1.0;

    normalize(grain.growth_tensor);
}
